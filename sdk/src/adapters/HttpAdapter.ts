/**
 * HttpAdapter
 *
 * HTTP-based transport adapter for communicating with a native plugin server.
 * Maps function names to HTTP endpoints and handles all network communication
 * including retries, timeouts, and error handling.
 *
 * This is a standalone transport class that the Bridge can optionally compose with.
 * It contains no application-specific logic -- endpoint maps, argument mappings,
 * and raw-body function sets are provided via constructor configuration.
 */

import type { EndpointDef, BridgeConfig } from '../bridge/types'

// ---------------------------------------------------------------------------
// Defaults
// ---------------------------------------------------------------------------

const DEFAULT_TIMEOUT = 10_000 // 10 seconds
const DEFAULT_MAX_RETRIES = 3
const DEFAULT_RETRY_DELAY = 1_000 // 1 second

// ---------------------------------------------------------------------------
// Config interface
// ---------------------------------------------------------------------------

/** Configuration accepted by the HttpAdapter constructor */
export interface HttpAdapterConfig {
  /** Base URL for the plugin HTTP server (e.g. "http://localhost:8080") */
  serverUrl: string
  /** Request timeout in milliseconds (default: 10 000) */
  timeout?: number
  /** Maximum retry attempts for transient failures (default: 3) */
  maxRetries?: number
  /** Base delay between retries in ms (default: 1 000) */
  retryDelay?: number
  /** Map of function names to endpoint definitions */
  endpointMap?: Record<string, EndpointDef>
  /** Positional-argument mappings: function name -> ordered param names */
  argMappings?: Record<string, string[]>
  /** Set of function names whose single string arg is sent as a raw body */
  rawBodyFunctions?: Set<string>
  /** Map of function names to static stub responses (skips HTTP entirely) */
  stubResponses?: Record<string, unknown>
}

// ---------------------------------------------------------------------------
// HttpAdapter
// ---------------------------------------------------------------------------

/**
 * HTTP adapter for calling host functions on a native plugin server.
 *
 * Usage:
 * ```ts
 * const http = new HttpAdapter({ serverUrl: 'http://localhost:8080' })
 * const result = await http.callHostFunction<MyType>('getDocument')
 * ```
 */
export class HttpAdapter {
  private serverUrl: string
  private timeout: number
  private maxRetries: number
  private retryDelay: number
  private connected = false

  private endpointMap: Record<string, EndpointDef>
  private argMappings: Record<string, string[]>
  private rawBodyFunctions: Set<string>
  private stubResponses: Record<string, unknown>

  constructor(config: HttpAdapterConfig) {
    this.serverUrl = config.serverUrl
    this.timeout = config.timeout ?? DEFAULT_TIMEOUT
    this.maxRetries = config.maxRetries ?? DEFAULT_MAX_RETRIES
    this.retryDelay = config.retryDelay ?? DEFAULT_RETRY_DELAY
    this.endpointMap = config.endpointMap ?? {}
    this.argMappings = config.argMappings ?? {}
    this.rawBodyFunctions = config.rawBodyFunctions ?? new Set()
    this.stubResponses = config.stubResponses ?? {}
  }

  // -----------------------------------------------------------------------
  // Public API
  // -----------------------------------------------------------------------

  /**
   * Call a host function by name, mapping it to the appropriate HTTP endpoint.
   *
   * @param functionName - Logical function name (e.g. "getDocument")
   * @param args         - Positional arguments forwarded to the endpoint
   * @returns Parsed response body typed as `T`
   */
  async callHostFunction<T>(functionName: string, ...args: unknown[]): Promise<T> {
    // Short-circuit for stubbed functions
    if (functionName in this.stubResponses) {
      return this.stubResponses[functionName] as T
    }

    const endpoint = this.endpointMap[functionName]

    if (!endpoint) {
      console.warn(`[HttpAdapter] Unknown function: ${functionName}, falling back to POST /host/call`)
      return this.callGenericEndpoint<T>(functionName, args)
    }

    // Build URL, substituting path parameters like {uuid}
    let path = endpoint.path
    const remainingArgs = [...args]

    const pathParams = path.match(/\{(\w+)\}/g)
    if (pathParams) {
      for (const param of pathParams) {
        if (remainingArgs.length > 0) {
          const value = remainingArgs.shift()
          path = path.replace(param, encodeURIComponent(String(value)))
        }
      }
    }

    // Determine request body from remaining args
    let body: unknown = null
    let isRawBody = false

    if (remainingArgs.length > 0) {
      const mapping = this.argMappings[functionName]
      const isSingleObject =
        remainingArgs.length === 1 &&
        typeof remainingArgs[0] === 'object' &&
        !Array.isArray(remainingArgs[0])

      if (mapping && !isSingleObject) {
        // Named positional arg mapping takes precedence
        body = this.mapArgsToBody(functionName, remainingArgs)
      } else if (
        this.rawBodyFunctions.has(functionName) &&
        remainingArgs.length === 1 &&
        typeof remainingArgs[0] === 'string'
      ) {
        body = remainingArgs[0]
        isRawBody = true
      } else if (isSingleObject) {
        body = remainingArgs[0]
      } else {
        body = this.mapArgsToBody(functionName, remainingArgs)
      }
    }

    // For GET requests convert body to query params
    let url = `${this.serverUrl}${path}`
    if (endpoint.method === 'GET' && body && typeof body === 'object') {
      const params = new URLSearchParams()
      for (const [key, value] of Object.entries(body as Record<string, unknown>)) {
        if (value !== undefined && value !== null) {
          params.append(key, String(value))
        }
      }
      const qs = params.toString()
      if (qs) url += `?${qs}`
      body = null
    }

    return this.fetchWithRetry<T>(url, {
      method: endpoint.method,
      headers: { 'Content-Type': 'application/json' },
      body: body != null ? (isRawBody ? (body as string) : JSON.stringify(body)) : undefined,
    })
  }

  /**
   * Test whether the adapter can reach the server.
   *
   * @returns `true` if the server responds to a health-check style call
   */
  async isConnected(): Promise<boolean> {
    try {
      await this.callHostFunction('testConnection')
      return true
    } catch {
      return false
    }
  }

  /** Get the current server URL. */
  getServerUrl(): string {
    return this.serverUrl
  }

  /** Update the server URL at runtime. */
  setServerUrl(url: string): void {
    this.serverUrl = url
  }

  /** Cached connection status (updated after every request). */
  get isCurrentlyConnected(): boolean {
    return this.connected
  }

  // -----------------------------------------------------------------------
  // Configuration setters (runtime hot-swap)
  // -----------------------------------------------------------------------

  /** Replace the endpoint map at runtime. */
  setEndpointMap(map: Record<string, EndpointDef>): void {
    this.endpointMap = map
  }

  /** Merge additional endpoints into the existing map. */
  mergeEndpointMap(map: Record<string, EndpointDef>): void {
    Object.assign(this.endpointMap, map)
  }

  /** Replace arg mappings at runtime. */
  setArgMappings(mappings: Record<string, string[]>): void {
    this.argMappings = mappings
  }

  /** Replace the raw-body function set at runtime. */
  setRawBodyFunctions(fns: Set<string>): void {
    this.rawBodyFunctions = fns
  }

  /** Replace stub responses at runtime. */
  setStubResponses(stubs: Record<string, unknown>): void {
    this.stubResponses = stubs
  }

  // -----------------------------------------------------------------------
  // Private helpers
  // -----------------------------------------------------------------------

  /**
   * Fallback for unmapped functions -- sends to a generic endpoint.
   */
  private async callGenericEndpoint<T>(functionName: string, args: unknown[]): Promise<T> {
    const url = `${this.serverUrl}/host/call`
    const body = { function: functionName, args }

    return this.fetchWithRetry<T>(url, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(body),
    })
  }

  /**
   * Convert positional arguments to a named-property object using the
   * configured arg mapping for `functionName`.
   */
  private mapArgsToBody(functionName: string, args: unknown[]): Record<string, unknown> {
    const mapping = this.argMappings[functionName]

    if (!mapping) {
      return args.length === 1 ? { data: args[0] } : { args }
    }

    // Unpack the CEP calling convention: callHost('fn', [a, b]) -> single array
    let effectiveArgs = args
    if (args.length === 1 && Array.isArray(args[0]) && mapping.length > 1) {
      effectiveArgs = args[0] as unknown[]
    }

    const body: Record<string, unknown> = {}
    mapping.forEach((key, index) => {
      if (index < effectiveArgs.length && effectiveArgs[index] !== undefined) {
        body[key] = effectiveArgs[index]
      }
    })

    return body
  }

  /**
   * Execute a fetch with automatic retry on transient failures.
   */
  private async fetchWithRetry<T>(
    url: string,
    options: RequestInit,
    retries: number = this.maxRetries,
  ): Promise<T> {
    const controller = new AbortController()
    const timeoutId = setTimeout(() => controller.abort(), this.timeout)

    try {
      const response = await fetch(url, { ...options, signal: controller.signal })
      clearTimeout(timeoutId)

      if (!response.ok) {
        const errorBody = await response.text()
        throw new Error(`HTTP ${response.status}: ${errorBody}`)
      }

      const text = await response.text()

      // Empty response
      if (!text || text.trim() === '') {
        this.connected = true
        return {} as T
      }

      // Attempt JSON parse
      let data: unknown
      try {
        data = JSON.parse(text)
      } catch {
        // Non-JSON -- return raw text
        this.connected = true
        return text as unknown as T
      }

      this.connected = true

      // Surface server-side errors
      if (data && typeof data === 'object' && (data as Record<string, unknown>).error) {
        throw new Error(String((data as Record<string, unknown>).error))
      }

      return data as T
    } catch (error: unknown) {
      clearTimeout(timeoutId)

      if (retries > 0 && this.isRetryableError(error)) {
        console.log(`[HttpAdapter] Retrying (${retries} left): ${url}`)
        await this.delay(this.retryDelay)
        return this.fetchWithRetry<T>(url, options, retries - 1)
      }

      this.connected = false
      throw error
    }
  }

  /**
   * Determine whether an error is transient and worth retrying.
   */
  private isRetryableError(error: unknown): boolean {
    if (!(error instanceof Error)) return false

    if (error.name === 'AbortError') return true

    const msg = error.message?.toLowerCase() ?? ''
    if (msg.includes('fetch')) return true
    if (msg.includes('network')) return true
    if (msg.includes('failed to fetch')) return true
    if (msg.includes('connection')) return true

    // 5xx server errors
    if (error.message?.includes('HTTP 5')) return true

    return false
  }

  /** Simple delay helper. */
  private delay(ms: number): Promise<void> {
    return new Promise(resolve => setTimeout(resolve, ms))
  }
}

// ---------------------------------------------------------------------------
// Factory helpers
// ---------------------------------------------------------------------------

/**
 * Create a new HttpAdapter from a partial BridgeConfig.
 *
 * @param config - BridgeConfig (typically from the bridge layer)
 * @returns A fully configured HttpAdapter instance
 */
export function createHttpAdapter(config: BridgeConfig): HttpAdapter {
  return new HttpAdapter({
    serverUrl: config.baseUrl ?? `http://localhost:${config.port ?? 8080}`,
    timeout: config.timeout,
    maxRetries: config.maxRetries,
    retryDelay: config.retryDelay,
    endpointMap: config.endpointMap,
    argMappings: config.argMappings,
    rawBodyFunctions: config.rawBodyFunctions,
  })
}
