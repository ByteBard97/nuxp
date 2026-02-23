/**
 * Bridge type definitions
 *
 * Configuration interfaces and types for the NUXP communication bridge.
 */

/** HTTP method types supported by endpoints */
export type HttpMethod = 'GET' | 'POST' | 'PUT' | 'DELETE' | 'PATCH'

/** Definition of a single endpoint */
export interface EndpointDef {
  method: HttpMethod
  path: string
}

/** Bridge configuration options */
export interface BridgeConfig {
  /** Base URL for the plugin HTTP server (e.g., "http://localhost:8080") */
  baseUrl?: string
  /** Port number for the plugin HTTP server */
  port?: number
  /** Request timeout in milliseconds (default: 10000) */
  timeout?: number
  /** Enable request serialization via AutoQueue (default: true) */
  queue?: boolean
  /** Whether to use mock mode */
  useMock?: boolean
  /** Maximum retry attempts for failed requests (default: 3) */
  maxRetries?: number
  /** Delay between retries in ms (default: 1000) */
  retryDelay?: number
  /** App-provided endpoint map (merged with any generated endpoints) */
  endpointMap?: Record<string, EndpointDef>
  /** Functions that send raw string body instead of JSON */
  rawBodyFunctions?: Set<string>
  /** Positional argument mappings for converting args to named body properties */
  argMappings?: Record<string, string[]>
}

/** Response structure from the C++ plugin (suite call pattern) */
export interface CppResponse<T = unknown> {
  success: boolean
  result?: T
  error?: string
  errorCode?: number
}

/** Request payload for suite-based calls */
export interface CppRequest {
  suite: string
  method: string
  args: Record<string, unknown>
}

/** Queue status for diagnostics */
export interface QueueStatus {
  length: number
  isBusy: boolean
}

/** Bridge environment info for diagnostics */
export interface BridgeEnvironmentInfo {
  mode: 'http'
  serverUrl: string
  connected: boolean
}
