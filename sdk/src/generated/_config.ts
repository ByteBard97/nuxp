/**
 * Compatibility shim for generated route/event files.
 *
 * The auto-generated `customRoutes.ts` and `events.ts` import
 * `getApiUrl` and `sdkConfig` from `../config` (a shell-specific module).
 * This shim provides those symbols backed by a shared Bridge instance
 * so the generated code works inside the SDK.
 *
 * Apps must call `setBridgeInstance()` from `./_bridge` at startup.
 */

import { getBridgeInstance } from './_bridge'

/**
 * Minimal sdkConfig-compatible object.
 * The generated events.ts reads `sdkConfig.useMock` to skip SSE in mock mode.
 */
export const sdkConfig = {
  get useMock(): boolean {
    return false // SDK bridge doesn't have a global mock flag; override if needed
  },
}

/**
 * Build a full API URL from a path, using the Bridge's current server URL.
 *
 * @param path - API path (e.g. "/api/call", "/events/stream")
 * @returns Fully qualified URL
 */
export function getApiUrl(path: string): string {
  const bridge = getBridgeInstance()
  const base = bridge.getServerUrl()
  const normalizedPath = path.startsWith('/') ? path : `/${path}`
  return `${base}${normalizedPath}`
}
