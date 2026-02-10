/**
 * SDK Configuration
 *
 * Central configuration for the TypeScript-to-C++ bridge.
 * Reads from Vite environment variables with sensible defaults.
 * Supports runtime port changes with localStorage persistence.
 */

import { NUXP_DEFAULT_PORT } from './defaults';

/**
 * Default port for the C++ plugin HTTP server
 * (Imported from CMake-generated defaults.ts)
 */
export const DEFAULT_PORT = NUXP_DEFAULT_PORT;

/**
 * Default host for the C++ plugin HTTP server
 */
export const DEFAULT_HOST = 'localhost';

/**
 * Default request timeout in milliseconds
 */
export const DEFAULT_TIMEOUT_MS = 10000;

/**
 * Default number of retry attempts for failed requests
 */
export const DEFAULT_RETRY_COUNT = 0;

/**
 * LocalStorage key for port persistence
 */
const STORAGE_KEY_PORT = 'nuxp_port';

/**
 * SDK configuration interface
 */
export interface SdkConfig {
  /** Base URL for the plugin API (e.g., "http://localhost:8080") */
  baseUrl: string;
  /** Current port number */
  port: number;
  /** Request timeout in milliseconds */
  timeoutMs: number;
  /** Number of retry attempts for failed requests */
  retryCount: number;
  /** Whether to use mock mode instead of real API calls */
  useMock: boolean;
}

/**
 * Get port from localStorage or return default
 */
function getSavedPort(): number {
  try {
    const saved = localStorage.getItem(STORAGE_KEY_PORT);
    if (saved) {
      const port = parseInt(saved, 10);
      if (port >= 1024 && port <= 65535) {
        return port;
      }
    }
  } catch {
    // localStorage may not be available (SSR, etc.)
  }
  return DEFAULT_PORT;
}

/**
 * Build the base URL from environment or defaults
 */
function getBaseUrl(port: number): string {
  const envUrl = import.meta.env.VITE_PLUGIN_URL;
  if (envUrl) {
    return envUrl;
  }
  return `http://${DEFAULT_HOST}:${port}`;
}

/**
 * Check if mock mode is enabled via environment variable
 */
function isMockEnabled(): boolean {
  return import.meta.env.VITE_USE_MOCK === 'true';
}

// Initialize port from localStorage
const initialPort = getSavedPort();

/**
 * Global SDK configuration instance
 * Initialized from environment variables and localStorage with defaults
 */
export const sdkConfig: SdkConfig = {
  port: initialPort,
  baseUrl: getBaseUrl(initialPort),
  timeoutMs: DEFAULT_TIMEOUT_MS,
  retryCount: DEFAULT_RETRY_COUNT,
  useMock: isMockEnabled(),
};

/**
 * Update SDK configuration at runtime
 * Useful for testing or dynamic reconfiguration
 *
 * @param updates - Partial configuration updates to apply
 */
export function updateSdkConfig(updates: Partial<SdkConfig>): void {
  Object.assign(sdkConfig, updates);
}

/**
 * Set the server port and update the base URL
 * Also persists to localStorage for page reloads
 *
 * @param port - New port number
 */
export function setPort(port: number): void {
  sdkConfig.port = port;
  sdkConfig.baseUrl = getBaseUrl(port);

  try {
    localStorage.setItem(STORAGE_KEY_PORT, String(port));
  } catch {
    // localStorage may not be available
  }
}

/**
 * Get the current port number
 */
export function getPort(): number {
  return sdkConfig.port;
}

/**
 * Get the full API endpoint URL for a given path
 *
 * @param path - API path (e.g., "/api/call")
 * @returns Full URL (e.g., "http://localhost:8080/api/call")
 */
export function getApiUrl(path: string): string {
  const normalizedPath = path.startsWith('/') ? path : `/${path}`;
  return `${sdkConfig.baseUrl}${normalizedPath}`;
}
