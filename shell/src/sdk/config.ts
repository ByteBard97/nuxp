/**
 * SDK Configuration
 *
 * Central configuration for the TypeScript-to-C++ bridge.
 * Reads from Vite environment variables with sensible defaults.
 */

/**
 * Default port for the C++ plugin HTTP server
 */
export const DEFAULT_PORT = 8080;

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
 * SDK configuration interface
 */
export interface SdkConfig {
  /** Base URL for the plugin API (e.g., "http://localhost:8080") */
  baseUrl: string;
  /** Request timeout in milliseconds */
  timeoutMs: number;
  /** Number of retry attempts for failed requests */
  retryCount: number;
  /** Whether to use mock mode instead of real API calls */
  useMock: boolean;
}

/**
 * Build the base URL from environment or defaults
 */
function getBaseUrl(): string {
  const envUrl = import.meta.env.VITE_PLUGIN_URL;
  if (envUrl) {
    return envUrl;
  }
  return `http://${DEFAULT_HOST}:${DEFAULT_PORT}`;
}

/**
 * Check if mock mode is enabled via environment variable
 */
function isMockEnabled(): boolean {
  return import.meta.env.VITE_USE_MOCK === 'true';
}

/**
 * Global SDK configuration instance
 * Initialized from environment variables with defaults
 */
export const sdkConfig: SdkConfig = {
  baseUrl: getBaseUrl(),
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
 * Get the full API endpoint URL for a given path
 *
 * @param path - API path (e.g., "/api/call")
 * @returns Full URL (e.g., "http://localhost:8080/api/call")
 */
export function getApiUrl(path: string): string {
  const normalizedPath = path.startsWith('/') ? path : `/${path}`;
  return `${sdkConfig.baseUrl}${normalizedPath}`;
}
