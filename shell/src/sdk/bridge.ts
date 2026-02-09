/**
 * TypeScript Bridge for C++ Plugin Communication
 *
 * This module provides the transport layer for calling C++ plugin methods
 * from TypeScript. It handles HTTP communication, JSON serialization,
 * error handling, and mock mode routing.
 *
 * Usage:
 *   import { callCpp } from '@/sdk/bridge';
 *   const result = await callCpp('AIArt', 'GetArtName', { art: artHandle });
 */

import { sdkConfig, getApiUrl } from './config';
import { mockCallCpp, isMockModeEnabled } from './mockBridge';
import { eventBus } from './events';

/**
 * Error thrown when a C++ plugin call fails
 */
export class BridgeError extends Error {
  constructor(
    message: string,
    public readonly suite: string,
    public readonly method: string,
    public readonly statusCode?: number,
    public readonly originalError?: unknown
  ) {
    super(message);
    this.name = 'BridgeError';
  }
}

/**
 * Response structure from the C++ plugin
 */
export interface CppResponse<T = unknown> {
  /** Whether the call succeeded */
  success: boolean;
  /** Result data if successful */
  result?: T;
  /** Error message if failed */
  error?: string;
  /** Error code if failed */
  errorCode?: number;
}

/**
 * Request payload sent to the C++ plugin
 */
interface CppRequest {
  suite: string;
  method: string;
  args: Record<string, unknown>;
}

/**
 * Create an AbortController with automatic timeout
 *
 * @param timeoutMs - Timeout in milliseconds
 * @returns AbortController that will abort after the timeout
 */
function createTimeoutController(timeoutMs: number): AbortController {
  const controller = new AbortController();
  const timeoutId = setTimeout(() => controller.abort(), timeoutMs);

  // Clear timeout if the controller is aborted early
  controller.signal.addEventListener('abort', () => clearTimeout(timeoutId));

  return controller;
}

/**
 * Call a C++ plugin method
 *
 * This is the main entry point for TypeScript-to-C++ communication.
 * When VITE_USE_MOCK=true, calls are routed to the mock bridge instead.
 *
 * @param suite - The suite name (e.g., "AIArt", "AILayer")
 * @param method - The method name within the suite (e.g., "GetArtName")
 * @param args - Arguments to pass to the method
 * @returns The result from the C++ method
 * @throws BridgeError if the call fails
 *
 * @example
 * ```typescript
 * // Get an art object's name
 * const name = await callCpp<string>('AIArt', 'GetArtName', { art: artHandle });
 *
 * // Get art bounds
 * const bounds = await callCpp<AIRealRect>('AIArt', 'GetArtBounds', { art: artHandle });
 * ```
 */
export async function callCpp<T = unknown>(
  suite: string,
  method: string,
  args: Record<string, unknown> = {}
): Promise<T> {
  // Route to mock bridge if mock mode is enabled
  if (sdkConfig.useMock || isMockModeEnabled()) {
    return mockCallCpp<T>(suite, method, args);
  }

  const url = getApiUrl('/api/call');
  const controller = createTimeoutController(sdkConfig.timeoutMs);

  const requestBody: CppRequest = {
    suite,
    method,
    args,
  };

  try {
    const response = await fetch(url, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        Accept: 'application/json',
      },
      body: JSON.stringify(requestBody),
      signal: controller.signal,
    });

    // Handle HTTP errors
    if (!response.ok) {
      let errorMessage = `HTTP ${response.status}: ${response.statusText}`;

      try {
        const errorBody = await response.json();
        if (errorBody.error) {
          errorMessage = errorBody.error;
        }
      } catch {
        // Ignore JSON parse errors for error responses
      }

      throw new BridgeError(
        errorMessage,
        suite,
        method,
        response.status
      );
    }

    // Parse the JSON response
    let data: CppResponse<T>;
    try {
      data = await response.json();
    } catch (parseError) {
      throw new BridgeError(
        'Failed to parse JSON response from plugin',
        suite,
        method,
        response.status,
        parseError
      );
    }

    // Check for application-level errors
    if (!data.success) {
      throw new BridgeError(
        data.error || 'Plugin call failed',
        suite,
        method,
        data.errorCode
      );
    }

    return data.result as T;
  } catch (error) {
    // Re-throw BridgeErrors as-is
    if (error instanceof BridgeError) {
      throw error;
    }

    // Handle abort/timeout errors
    if (error instanceof Error) {
      if (error.name === 'AbortError') {
        throw new BridgeError(
          `Request timed out after ${sdkConfig.timeoutMs}ms`,
          suite,
          method,
          undefined,
          error
        );
      }

      // Handle network errors
      if (error.message.includes('Failed to fetch') || error.message.includes('NetworkError')) {
        throw new BridgeError(
          `Network error: Unable to connect to plugin at ${url}. Is the plugin running?`,
          suite,
          method,
          undefined,
          error
        );
      }

      throw new BridgeError(
        `Request failed: ${error.message}`,
        suite,
        method,
        undefined,
        error
      );
    }

    // Handle unknown errors
    throw new BridgeError(
      'Unknown error occurred',
      suite,
      method,
      undefined,
      error
    );
  }
}

/**
 * Batch multiple C++ calls together
 *
 * This can be more efficient when making multiple related calls,
 * as it may reduce HTTP overhead in future implementations.
 *
 * @param calls - Array of calls to make
 * @returns Array of results in the same order as the calls
 */
export async function callCppBatch<T extends unknown[]>(
  calls: Array<{ suite: string; method: string; args?: Record<string, unknown> }>
): Promise<T> {
  // For now, execute calls in parallel
  // Future: could use a batch endpoint if the plugin supports it
  const promises = calls.map((call) =>
    callCpp(call.suite, call.method, call.args || {})
  );

  return Promise.all(promises) as Promise<T>;
}

/**
 * Check if the plugin is reachable
 *
 * @returns true if the plugin responds to health checks
 */
export async function isPluginAvailable(): Promise<boolean> {
  if (sdkConfig.useMock || isMockModeEnabled()) {
    return true;
  }

  try {
    const url = getApiUrl('/health');
    const controller = createTimeoutController(2000); // Short timeout for health check

    const response = await fetch(url, {
      method: 'GET',
      signal: controller.signal,
    });

    return response.ok;
  } catch {
    return false;
  }
}

/**
 * Event Loop State
 */
let isPolling = false;

/**
 * Start the event polling loop
 *
 * Polls the /events endpoint for updates from Illustrator.
 * Dispatches received events to the eventBus.
 */
export async function startEventLoop() {
  // Don't start if already polling or in mock mode
  if (isPolling || sdkConfig.useMock || isMockModeEnabled()) {
    return;
  }

  isPolling = true;
  console.log('[Bridge] Starting event loop...');

  while (isPolling) {
    try {
      const url = getApiUrl('/events');

      // Server waits up to 1s, so we set a slighly larger timeout
      const controller = createTimeoutController(2000);

      const response = await fetch(url, {
        method: 'GET',
        headers: {
          'Accept': 'application/json'
        },
        signal: controller.signal,
      });

      if (response.ok) {
        const data = await response.json();
        if (data.success && Array.isArray(data.events)) {
          // Dispatch each event
          for (const evt of data.events) {
            if (evt.type) {
              eventBus.emit(evt.type, evt.data);
            }
          }
        }
      } else {
        // If server returns error (e.g. 500), wait a bit before retrying
        await new Promise(resolve => setTimeout(resolve, 1000));
      }
    } catch (error) {
      // Network error (e.g. fetch failed) or timeout - wait and retry
      if (isPolling) {
        await new Promise(resolve => setTimeout(resolve, 1000));
      }
    }
  }

  console.log('[Bridge] Event loop stopped');
}

/**
 * Stop the event polling loop
 */
export function stopEventLoop() {
  isPolling = false;
}

