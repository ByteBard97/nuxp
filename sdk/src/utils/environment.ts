/**
 * Environment Detection Utilities
 *
 * Detects the runtime environment: Tauri (standalone desktop app) or Browser (dev).
 * Provides helpers for determining the C++ plugin HTTP server URL.
 */

/** Default port for the C++ plugin HTTP server */
const DEFAULT_PORT = 8081

/**
 * Check if running inside a Tauri desktop application.
 * - `window.__TAURI__` is the runtime object injected by Tauri
 * - `__TAURI__` is a Vite compile-time define
 */
declare const __TAURI__: boolean | undefined
export const isTauri: boolean =
  typeof (window as any).__TAURI__ !== 'undefined' ||
  (typeof __TAURI__ !== 'undefined' && __TAURI__ === true)

/**
 * Runtime environment discriminator.
 */
export type Environment = 'tauri' | 'browser'

/**
 * Detect the current runtime environment.
 */
export function getEnvironment(): Environment {
  if (isTauri) return 'tauri'
  return 'browser'
}

/**
 * Current environment as a constant (evaluated once at module load).
 */
export const environment: Environment = getEnvironment()

/**
 * Check if running in browser mode (development/testing).
 */
export const isBrowser: boolean = environment === 'browser'

/**
 * Build the default HTTP server URL for a given port.
 *
 * @param port - Port number (defaults to 8081)
 * @returns Full URL string, e.g. `http://localhost:8081`
 */
export function buildServerUrl(port: number = DEFAULT_PORT): string {
  return `http://localhost:${port}`
}

/**
 * Get the HTTP server URL for the C++ plugin.
 *
 * Resolution order:
 * 1. Explicit `port` parameter
 * 2. `window.__NUXP_SERVER_URL__` (set by the Tauri host)
 * 3. Default (`http://localhost:8081`)
 *
 * @param port - Optional port override
 * @returns The server base URL
 */
export function getServerUrl(port?: number): string {
  if (port !== undefined) {
    return buildServerUrl(port)
  }
  if (isTauri) {
    return (window as any).__NUXP_SERVER_URL__ || buildServerUrl()
  }
  return buildServerUrl()
}

/**
 * Default HTTP server URL (evaluated once at module load).
 */
export const HTTP_SERVER_URL: string = getServerUrl()
