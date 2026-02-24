/**
 * Compatibility shim for generated suite files.
 *
 * The auto-generated suite modules import `callCpp` from `@/sdk/bridge`
 * (a demo-app path alias). This shim re-exports a `callCpp` function
 * that delegates to a shared Bridge instance so the generated code works
 * inside the SDK without rewriting every import.
 *
 * Apps must call `setBridgeInstance()` at startup to wire up the real bridge.
 */

import { Bridge, createBridge } from '../bridge/Bridge'

/** Module-level bridge reference (lazy-initialized with defaults). */
let _bridge: Bridge = createBridge()

/**
 * Set the Bridge instance used by all generated suite files.
 *
 * Call this once at app startup after creating your configured Bridge:
 *
 * ```ts
 * import { createBridge } from '@nuxp/sdk'
 * import { setBridgeInstance } from '@nuxp/sdk/generated/_bridge'
 *
 * const bridge = createBridge({ port: 8080 })
 * setBridgeInstance(bridge)
 * ```
 */
export function setBridgeInstance(bridge: Bridge): void {
  _bridge = bridge
}

/**
 * Get the current Bridge instance.
 */
export function getBridgeInstance(): Bridge {
  return _bridge
}

/**
 * Compatibility wrapper matching the original `callCpp` signature.
 *
 * Delegates to `Bridge.callSuite()` so every generated suite file
 * continues to work without modification.
 */
export async function callCpp<T = unknown>(
  suite: string,
  method: string,
  args: Record<string, unknown> = {},
): Promise<T> {
  return _bridge.callSuite<T>(suite, method, args)
}
