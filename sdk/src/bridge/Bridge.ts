/**
 * Bridge - Unified communication layer for NUXP plugin interaction
 *
 * Merges three communication patterns into a single, framework-agnostic class:
 *
 * 1. **Suite calls** (from nuxp/shell bridge) — `callSuite('AIArt', 'GetArtName', { art })`
 *    Direct POST to /api/call with typed {suite, method, args} bodies.
 *
 * 2. **Endpoint-mapped calls** (from HttpApiAdapter) — `call('getDocument')`
 *    Function names resolve to HTTP endpoints via a configurable map, with
 *    path-parameter substitution, positional arg mapping, and retry logic.
 *
 * 3. **Queued execution** (from HostBridge) — all calls optionally serialize
 *    through an AutoQueue so the single-threaded host is never overwhelmed.
 *
 * The Bridge is fully self-contained: no Vue, no Pinia, no framework imports.
 * App-specific endpoint maps, arg mappings, and raw-body function sets are
 * injected via BridgeConfig — nothing is hardcoded.
 */

import type {
  BridgeConfig,
  BridgeEnvironmentInfo,
  CppRequest,
  CppResponse,
  EndpointDef,
  HttpMethod,
  QueueStatus,
} from './types'
import { AutoQueue } from './AutoQueue'

// ─── Defaults ──────────────────────────────────────────────────────────────────

const DEFAULT_PORT = 8080
const DEFAULT_TIMEOUT = 10_000
const DEFAULT_MAX_RETRIES = 3
const DEFAULT_RETRY_DELAY = 1_000
const HEALTH_CHECK_TIMEOUT = 2_000

// ─── BridgeError ───────────────────────────────────────────────────────────────

/**
 * Error thrown when a bridge call fails.
 *
 * Captures contextual information (suite/method or function name, HTTP status)
 * so callers can inspect the failure without parsing message strings.
 */
export class BridgeError extends Error {
  public readonly name = 'BridgeError'

  constructor(
    message: string,
    /** Suite name (suite call pattern) or function name (endpoint call pattern) */
    public readonly suite: string,
    /** Method name (suite call pattern) or empty string for endpoint calls */
    public readonly method: string,
    /** HTTP status code, or application-level error code */
    public readonly statusCode?: number,
    /** The underlying error, if any */
    public readonly originalError?: unknown,
  ) {
    super(message)
  }
}

// ─── Bridge ────────────────────────────────────────────────────────────────────

/**
 * Unified communication bridge for the NUXP SDK.
 *
 * @example
 * ```ts
 * const bridge = createBridge({ port: 3000, queue: true })
 *
 * // Endpoint-mapped call
 * const doc = await bridge.call<Document>('getDocument')
 *
 * // Suite call (Illustrator SDK pattern)
 * const name = await bridge.callSuite<string>('AIArt', 'GetArtName', { art: handle })
 *
 * // Parallel batch
 * const [a, b] = await bridge.batch([
 *   { suite: 'AIArt', method: 'GetArtName', args: { art: h1 } },
 *   { suite: 'AIArt', method: 'GetArtName', args: { art: h2 } },
 * ])
 * ```
 */
export class Bridge {
  private serverUrl: string
  private timeout: number
  private maxRetries: number
  private retryDelay: number
  private queueEnabled: boolean
  private queue: AutoQueue
  private connected: boolean = false

  /** Runtime endpoint map (function name -> HTTP method + path) */
  private endpointMap: Record<string, EndpointDef>

  /** Functions that send a raw string body instead of JSON-serialized object */
  private rawBodyFunctions: Set<string>

  /** Positional argument name mappings (function name -> ordered param names) */
  private argMappings: Record<string, string[]>

  constructor(config: BridgeConfig = {}) {
    const baseUrl = config.baseUrl ?? `http://localhost:${config.port ?? DEFAULT_PORT}`
    this.serverUrl = baseUrl.replace(/\/+$/, '') // strip trailing slashes
    this.timeout = config.timeout ?? DEFAULT_TIMEOUT
    this.maxRetries = config.maxRetries ?? DEFAULT_MAX_RETRIES
    this.retryDelay = config.retryDelay ?? DEFAULT_RETRY_DELAY
    this.queueEnabled = config.queue ?? true
    this.queue = new AutoQueue()
    this.endpointMap = { ...(config.endpointMap ?? {}) }
    this.rawBodyFunctions = new Set(config.rawBodyFunctions ?? [])
    this.argMappings = { ...(config.argMappings ?? {}) }
  }

  // ─── Public API ────────────────────────────────────────────────────────────

  /**
   * Call a function by name, resolving it through the endpoint map.
   *
   * If the function name exists in the endpoint map the request is built from
   * that definition (method, path, path params, body/query conversion). If no
   * mapping is found, a generic `POST /host/call` fallback is used.
   *
   * When queuing is enabled the underlying fetch is serialized through AutoQueue.
   *
   * @param functionName - Registered function name (e.g. "getDocument", "deselectAll")
   * @param args - Positional arguments; mapped to named body properties via argMappings
   * @returns Parsed response of type T
   * @throws {BridgeError} on HTTP, network, timeout, or application-level errors
   */
  public call<T = unknown>(functionName: string, ...args: unknown[]): Promise<T> {
    if (this.queueEnabled) {
      return this.queue.enqueue(() => this.executeCall<T>(functionName, args))
    }
    return this.executeCall<T>(functionName, args)
  }

  /**
   * Direct suite/method call pattern (NUXP Illustrator SDK style).
   *
   * Sends `POST /api/call` with `{ suite, method, args }` body and unwraps
   * the `CppResponse<T>` envelope.
   *
   * @param suite - Suite name (e.g. "AIArt", "AILayer")
   * @param method - Method name within the suite
   * @param args - Named arguments passed to the method
   * @returns The unwrapped result of type T
   * @throws {BridgeError} on failure
   */
  public callSuite<T = unknown>(
    suite: string,
    method: string,
    args: Record<string, unknown> = {},
  ): Promise<T> {
    const execute = () => this.executeSuiteCall<T>(suite, method, args)
    if (this.queueEnabled) {
      return this.queue.enqueue(execute)
    }
    return execute()
  }

  /**
   * Execute multiple suite calls in parallel.
   *
   * Each call in the array is dispatched concurrently (not serialized through
   * the queue). Useful when the results are independent and you want to
   * minimize total wall-clock time.
   *
   * @param calls - Array of suite call descriptors
   * @returns Tuple of results in the same order as the input calls
   */
  public batch<T extends unknown[]>(
    calls: Array<{ suite: string; method: string; args?: Record<string, unknown> }>,
  ): Promise<T> {
    const promises = calls.map((c) =>
      this.executeSuiteCall(c.suite, c.method, c.args ?? {}),
    )
    return Promise.all(promises) as Promise<T>
  }

  /**
   * Merge additional endpoints into the runtime map.
   *
   * Later registrations override earlier ones for the same function name,
   * so app-specific routes can shadow defaults.
   *
   * @param map - Record of function name to endpoint definition
   */
  public registerEndpoints(map: Record<string, EndpointDef>): void {
    Object.assign(this.endpointMap, map)
  }

  /**
   * Register additional positional-argument mappings at runtime.
   *
   * @param mappings - Record of function name to ordered parameter names
   */
  public registerArgMappings(mappings: Record<string, string[]>): void {
    Object.assign(this.argMappings, mappings)
  }

  /**
   * Check whether the plugin HTTP server is reachable.
   *
   * Issues a lightweight `GET /health` with a short timeout.
   *
   * @returns `true` if the server responds with 2xx, `false` otherwise
   */
  public async isAvailable(): Promise<boolean> {
    try {
      const controller = new AbortController()
      const timeoutId = setTimeout(() => controller.abort(), HEALTH_CHECK_TIMEOUT)

      const response = await fetch(`${this.serverUrl}/health`, {
        method: 'GET',
        signal: controller.signal,
      })

      clearTimeout(timeoutId)
      this.connected = response.ok
      return response.ok
    } catch {
      this.connected = false
      return false
    }
  }

  /**
   * Return the current AutoQueue status for diagnostics.
   */
  public getQueueStatus(): QueueStatus {
    return {
      length: this.queue.length,
      isBusy: this.queue.isBusy,
    }
  }

  /**
   * Get the current base server URL.
   */
  public getServerUrl(): string {
    return this.serverUrl
  }

  /**
   * Update the base server URL at runtime.
   *
   * @param url - New base URL (trailing slashes are stripped)
   */
  public setServerUrl(url: string): void {
    this.serverUrl = url.replace(/\/+$/, '')
  }

  /**
   * Return diagnostic environment information.
   */
  public getEnvironmentInfo(): BridgeEnvironmentInfo {
    return {
      mode: 'http',
      serverUrl: this.serverUrl,
      connected: this.connected,
    }
  }

  // ─── Private: endpoint-mapped call execution ────────────────────────────────

  /**
   * Execute an endpoint-mapped call (the core of `call()`).
   */
  private async executeCall<T>(functionName: string, args: unknown[]): Promise<T> {
    const endpoint = this.endpointMap[functionName]

    if (!endpoint) {
      return this.callGenericEndpoint<T>(functionName, args)
    }

    // Build URL, substituting path parameters from leading args
    const { url, remainingArgs } = this.buildRequestUrl(endpoint, args)

    // Build body from the remaining args
    const { body, isRawBody } = this.buildRequestBody(functionName, remainingArgs)

    // For GET requests, convert the body object to query parameters
    let finalUrl = url
    let finalBody: string | undefined

    if (endpoint.method === 'GET' && body !== null) {
      finalUrl = this.appendQueryParams(url, body as Record<string, unknown>)
      finalBody = undefined
    } else if (body !== null) {
      finalBody = isRawBody ? (body as string) : JSON.stringify(body)
    }

    return this.fetchWithRetry<T>(finalUrl, {
      method: endpoint.method,
      headers: { 'Content-Type': 'application/json' },
      body: finalBody,
    })
  }

  /**
   * Fallback for function names that have no endpoint mapping.
   *
   * Sends `POST /host/call` with `{ function, args }`.
   */
  private async callGenericEndpoint<T>(functionName: string, args: unknown[]): Promise<T> {
    const url = `${this.serverUrl}/host/call`
    const payload = { function: functionName, args }

    return this.fetchWithRetry<T>(url, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(payload),
    })
  }

  // ─── Private: suite call execution ──────────────────────────────────────────

  /**
   * Execute a suite/method call and unwrap the CppResponse envelope.
   */
  private async executeSuiteCall<T>(
    suite: string,
    method: string,
    args: Record<string, unknown>,
  ): Promise<T> {
    const url = `${this.serverUrl}/api/call`
    const requestBody: CppRequest = { suite, method, args }

    const controller = new AbortController()
    const timeoutId = setTimeout(() => controller.abort(), this.timeout)

    try {
      const response = await fetch(url, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          Accept: 'application/json',
        },
        body: JSON.stringify(requestBody),
        signal: controller.signal,
      })

      clearTimeout(timeoutId)

      if (!response.ok) {
        const errorMessage = await this.extractHttpErrorMessage(response)
        throw new BridgeError(errorMessage, suite, method, response.status)
      }

      let data: CppResponse<T>
      try {
        data = await response.json()
      } catch (parseError) {
        throw new BridgeError(
          'Failed to parse JSON response from plugin',
          suite,
          method,
          response.status,
          parseError,
        )
      }

      if (!data.success) {
        throw new BridgeError(
          data.error ?? 'Plugin call failed',
          suite,
          method,
          data.errorCode,
        )
      }

      this.connected = true
      return data.result as T
    } catch (error) {
      clearTimeout(timeoutId)

      if (error instanceof BridgeError) {
        throw error
      }

      throw this.wrapError(error, suite, method)
    }
  }

  // ─── Private: HTTP helpers ──────────────────────────────────────────────────

  /**
   * Fetch with automatic retry on transient failures.
   *
   * Retries on network errors, timeouts (AbortError), and 5xx status codes
   * up to `maxRetries` times with a fixed delay between attempts.
   *
   * @param url - Fully-qualified URL
   * @param options - Standard RequestInit (method, headers, body, etc.)
   * @param retriesLeft - Remaining retry attempts
   * @returns Parsed response of type T
   */
  private async fetchWithRetry<T>(
    url: string,
    options: RequestInit,
    retriesLeft: number = this.maxRetries,
  ): Promise<T> {
    const controller = new AbortController()
    const timeoutId = setTimeout(() => controller.abort(), this.timeout)

    try {
      const response = await fetch(url, {
        ...options,
        signal: controller.signal,
      })

      clearTimeout(timeoutId)

      if (!response.ok) {
        const errorBody = await response.text()
        throw new Error(`HTTP ${response.status}: ${errorBody}`)
      }

      const text = await response.text()

      // Handle empty responses
      if (!text || text.trim() === '') {
        this.connected = true
        return {} as T
      }

      // Attempt JSON parse; fall back to raw text
      let data: unknown
      try {
        data = JSON.parse(text)
      } catch {
        this.connected = true
        return text as unknown as T
      }

      this.connected = true

      // Surface application-level errors embedded in the JSON envelope
      if (data && typeof data === 'object' && (data as Record<string, unknown>).error) {
        throw new Error(String((data as Record<string, unknown>).error))
      }

      return data as T
    } catch (error) {
      clearTimeout(timeoutId)

      if (retriesLeft > 0 && this.isRetryableError(error)) {
        await this.delay(this.retryDelay)
        return this.fetchWithRetry<T>(url, options, retriesLeft - 1)
      }

      this.connected = false
      throw error
    }
  }

  // ─── Private: request building ──────────────────────────────────────────────

  /**
   * Build the full URL from an endpoint definition, substituting path
   * parameters (e.g. `{uuid}`) with leading positional arguments.
   *
   * @returns The constructed URL and any remaining (unconsumed) arguments.
   */
  private buildRequestUrl(
    endpoint: EndpointDef,
    args: unknown[],
  ): { url: string; remainingArgs: unknown[] } {
    let path = endpoint.path
    const remaining = [...args]

    const pathParams = path.match(/\{(\w+)\}/g)
    if (pathParams) {
      for (const param of pathParams) {
        if (remaining.length > 0) {
          const value = remaining.shift()
          path = path.replace(param, encodeURIComponent(String(value)))
        }
      }
    }

    return { url: `${this.serverUrl}${path}`, remainingArgs: remaining }
  }

  /**
   * Build the request body from remaining positional arguments.
   *
   * Resolution order:
   * 1. If argMappings exist and args are not a single object -> map positionally
   * 2. If the function is a raw-body function and a single string arg -> raw body
   * 3. If a single object arg -> pass through as-is
   * 4. Otherwise -> fall back to positional mapping (or generic wrapper)
   *
   * @returns The body value and whether it should be sent as a raw string.
   */
  private buildRequestBody(
    functionName: string,
    args: unknown[],
  ): { body: unknown; isRawBody: boolean } {
    if (args.length === 0) {
      return { body: null, isRawBody: false }
    }

    const isSingleObject =
      args.length === 1 && typeof args[0] === 'object' && args[0] !== null && !Array.isArray(args[0])

    // argMappings take precedence (unless the caller already passed a single object)
    if (this.argMappings[functionName] && !isSingleObject) {
      return { body: this.mapArgsToBody(functionName, args), isRawBody: false }
    }

    // Raw body functions: pass through a single string argument verbatim
    if (
      this.rawBodyFunctions.has(functionName) &&
      args.length === 1 &&
      typeof args[0] === 'string'
    ) {
      return { body: args[0], isRawBody: true }
    }

    // Single object -> pass through
    if (isSingleObject) {
      return { body: args[0], isRawBody: false }
    }

    // Default: attempt positional mapping, then generic wrapper
    return { body: this.mapArgsToBody(functionName, args), isRawBody: false }
  }

  /**
   * Convert positional arguments into a named-property object using argMappings.
   *
   * If no mapping is registered for `functionName`, falls back to a generic
   * `{ data }` (single arg) or `{ args }` (multiple args) wrapper.
   */
  private mapArgsToBody(functionName: string, args: unknown[]): Record<string, unknown> {
    const mapping = this.argMappings[functionName]

    if (!mapping) {
      if (args.length === 1) {
        return { data: args[0] }
      }
      return { args }
    }

    // Handle CEP-style single-array convention: callHost('fn', [a, b, c])
    let effectiveArgs: unknown[] = args
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
   * Append an object's entries as URL query parameters.
   */
  private appendQueryParams(url: string, params: Record<string, unknown>): string {
    const searchParams = new URLSearchParams()
    for (const [key, value] of Object.entries(params)) {
      if (value !== undefined && value !== null) {
        searchParams.append(key, String(value))
      }
    }
    const qs = searchParams.toString()
    return qs ? `${url}?${qs}` : url
  }

  // ─── Private: error handling ────────────────────────────────────────────────

  /**
   * Attempt to extract a human-readable error message from a non-OK HTTP response.
   */
  private async extractHttpErrorMessage(response: Response): Promise<string> {
    let message = `HTTP ${response.status}: ${response.statusText}`
    try {
      const body = await response.json()
      if (body?.error) {
        message = String(body.error)
      }
    } catch {
      // ignore — use the default status text message
    }
    return message
  }

  /**
   * Determine whether an error is transient and safe to retry.
   *
   * Retryable conditions:
   * - AbortError (request timed out)
   * - Network / fetch failures
   * - 5xx server errors
   */
  private isRetryableError(error: unknown): boolean {
    if (!(error instanceof Error)) return false

    if (error.name === 'AbortError') return true

    const msg = error.message.toLowerCase()
    if (msg.includes('fetch')) return true
    if (msg.includes('network')) return true
    if (msg.includes('failed to fetch')) return true
    if (msg.includes('connection')) return true

    // 5xx server errors
    if (error.message.includes('HTTP 5')) return true

    return false
  }

  /**
   * Wrap an unknown error in a BridgeError with context.
   */
  private wrapError(error: unknown, suite: string, method: string): BridgeError {
    if (error instanceof Error) {
      if (error.name === 'AbortError') {
        return new BridgeError(
          `Request timed out after ${this.timeout}ms`,
          suite,
          method,
          undefined,
          error,
        )
      }

      if (error.message.includes('Failed to fetch') || error.message.includes('NetworkError')) {
        return new BridgeError(
          `Network error: Unable to connect to plugin at ${this.serverUrl}. Is the plugin running?`,
          suite,
          method,
          undefined,
          error,
        )
      }

      return new BridgeError(
        `Request failed: ${error.message}`,
        suite,
        method,
        undefined,
        error,
      )
    }

    return new BridgeError('Unknown error occurred', suite, method, undefined, error)
  }

  /**
   * Promise-based delay for retry backoff.
   */
  private delay(ms: number): Promise<void> {
    return new Promise((resolve) => setTimeout(resolve, ms))
  }
}

// ─── Factory ───────────────────────────────────────────────────────────────────

/**
 * Create a new Bridge instance with the given configuration.
 *
 * This is the recommended way to instantiate a Bridge; apps can create
 * one at startup and pass it through their dependency-injection layer.
 *
 * @param config - Optional bridge configuration (all fields have sensible defaults)
 * @returns A configured Bridge instance
 *
 * @example
 * ```ts
 * import { createBridge } from '@nuxp/sdk/bridge'
 *
 * const bridge = createBridge({
 *   port: 3000,
 *   endpointMap: generatedEndpoints,
 *   argMappings: generatedArgMappings,
 * })
 * ```
 */
export function createBridge(config?: BridgeConfig): Bridge {
  return new Bridge(config)
}
