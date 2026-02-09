/**
 * Base HTTP client for plugin communication
 *
 * The NUXP plugin runs an HTTP server on localhost:8080 that accepts
 * JSON-RPC style requests. This module provides low-level communication
 * primitives used by the Illustrator service.
 *
 * When VITE_USE_MOCK=true, all API calls are intercepted and routed
 * to the MockBridge, returning realistic fake data for frontend development.
 */

import type { ApiResponse, HealthResponse } from './types';
import {
  isMockMode,
  mockCallPlugin,
  mockGetFromPlugin,
  mockCheckHealth,
  mockGetHealthInfo,
} from './MockBridge';

/** Whether mock mode is enabled */
const USE_MOCK = isMockMode();

/** Default plugin server URL */
const PLUGIN_URL = import.meta.env.VITE_PLUGIN_URL || 'http://localhost:8080';

// Log mock mode status on module load
if (USE_MOCK) {
  console.log('[API] Mock mode enabled - using MockBridge for all API calls');
}

/** Request timeout in milliseconds */
const REQUEST_TIMEOUT = 5000;

/**
 * API error class for plugin communication failures
 */
export class ApiError extends Error {
  constructor(
    message: string,
    public readonly statusCode?: number,
    public readonly endpoint?: string
  ) {
    super(message);
    this.name = 'ApiError';
  }
}

/**
 * Create an AbortController with timeout
 */
function createTimeoutController(timeoutMs: number): AbortController {
  const controller = new AbortController();
  setTimeout(() => controller.abort(), timeoutMs);
  return controller;
}

/**
 * Call a plugin endpoint with optional data
 *
 * @param endpoint - API endpoint path (e.g., '/document/info')
 * @param data - Optional request body data
 * @returns Parsed JSON response
 * @throws ApiError if request fails
 */
export async function callPlugin<T = unknown>(
  endpoint: string,
  data?: unknown
): Promise<ApiResponse<T>> {
  // Route to mock bridge when mock mode is enabled
  if (USE_MOCK) {
    return mockCallPlugin<T>(endpoint, data);
  }

  const url = `${PLUGIN_URL}${endpoint.startsWith('/') ? endpoint : '/' + endpoint}`;
  const controller = createTimeoutController(REQUEST_TIMEOUT);

  try {
    const response = await fetch(url, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json',
      },
      body: data !== undefined ? JSON.stringify(data) : undefined,
      signal: controller.signal,
    });

    if (!response.ok) {
      const errorText = await response.text().catch(() => 'Unknown error');
      throw new ApiError(
        `Plugin request failed: ${response.status} ${response.statusText} - ${errorText}`,
        response.status,
        endpoint
      );
    }

    const result = await response.json();
    return {
      success: true,
      data: result as T,
    };
  } catch (error) {
    if (error instanceof ApiError) {
      throw error;
    }

    if (error instanceof Error) {
      if (error.name === 'AbortError') {
        throw new ApiError(`Request timed out after ${REQUEST_TIMEOUT}ms`, undefined, endpoint);
      }
      throw new ApiError(`Plugin request failed: ${error.message}`, undefined, endpoint);
    }

    throw new ApiError('Unknown error occurred', undefined, endpoint);
  }
}

/**
 * Perform a GET request to a plugin endpoint
 *
 * @param endpoint - API endpoint path
 * @returns Parsed JSON response
 * @throws ApiError if request fails
 */
export async function getFromPlugin<T = unknown>(endpoint: string): Promise<ApiResponse<T>> {
  // Route to mock bridge when mock mode is enabled
  if (USE_MOCK) {
    return mockGetFromPlugin<T>(endpoint);
  }

  const url = `${PLUGIN_URL}${endpoint.startsWith('/') ? endpoint : '/' + endpoint}`;
  const controller = createTimeoutController(REQUEST_TIMEOUT);

  try {
    const response = await fetch(url, {
      method: 'GET',
      headers: {
        'Accept': 'application/json',
      },
      signal: controller.signal,
    });

    if (!response.ok) {
      const errorText = await response.text().catch(() => 'Unknown error');
      throw new ApiError(
        `Plugin request failed: ${response.status} ${response.statusText} - ${errorText}`,
        response.status,
        endpoint
      );
    }

    const result = await response.json();
    return {
      success: true,
      data: result as T,
    };
  } catch (error) {
    if (error instanceof ApiError) {
      throw error;
    }

    if (error instanceof Error) {
      if (error.name === 'AbortError') {
        throw new ApiError(`Request timed out after ${REQUEST_TIMEOUT}ms`, undefined, endpoint);
      }
      throw new ApiError(`Plugin request failed: ${error.message}`, undefined, endpoint);
    }

    throw new ApiError('Unknown error occurred', undefined, endpoint);
  }
}

/**
 * Check if the plugin is running and responsive
 *
 * @returns true if plugin responds to health check, false otherwise
 */
export async function checkHealth(): Promise<boolean> {
  // Route to mock bridge when mock mode is enabled
  if (USE_MOCK) {
    return mockCheckHealth();
  }

  try {
    const response = await getFromPlugin<HealthResponse>('/health');
    return response.success && response.data?.status === 'ok';
  } catch {
    return false;
  }
}

/**
 * Get plugin health information including version
 *
 * @returns Health response with version info, or null if unavailable
 */
export async function getHealthInfo(): Promise<HealthResponse | null> {
  // Route to mock bridge when mock mode is enabled
  if (USE_MOCK) {
    return mockGetHealthInfo();
  }

  try {
    const response = await getFromPlugin<HealthResponse>('/health');
    return response.data ?? null;
  } catch {
    return null;
  }
}

/**
 * Get the base plugin URL
 */
export function getPluginUrl(): string {
  return PLUGIN_URL;
}
