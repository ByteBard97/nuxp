/**
 * PluginAdapter
 *
 * Unified adapter that composes HttpAdapter + SSEAdapter into a single
 * entry-point for host communication. Provides environment detection,
 * a unified `callHostFunction` method, and event subscription helpers.
 *
 * Self-contained: no framework or application-specific imports.
 */

import { HttpAdapter, type HttpAdapterConfig } from './HttpAdapter'
import { SSEAdapter, type SSEAdapterConfig, type EventName, type EventCallback, type WildcardCallback } from './SSEAdapter'

// ---------------------------------------------------------------------------
// Environment detection
// ---------------------------------------------------------------------------

/** Detected runtime environment. */
export type PluginEnvironment = 'tauri' | 'browser' | 'cep'

// ---------------------------------------------------------------------------
// Config
// ---------------------------------------------------------------------------

/** Configuration for creating a PluginAdapter from scratch. */
export interface PluginAdapterConfig {
  /** Base URL for the plugin HTTP server */
  serverUrl: string
  /** Optional HTTP adapter config overrides */
  http?: Partial<Omit<HttpAdapterConfig, 'serverUrl'>>
  /** Optional SSE adapter config overrides */
  sse?: Partial<Omit<SSEAdapterConfig, 'serverUrl'>>
}

/** Configuration for creating a PluginAdapter from pre-built adapters. */
export interface PluginAdapterInstances {
  /** Pre-configured HttpAdapter instance */
  httpAdapter: HttpAdapter
  /** Pre-configured SSEAdapter instance */
  sseAdapter: SSEAdapter
}

// ---------------------------------------------------------------------------
// PluginAdapter
// ---------------------------------------------------------------------------

/**
 * Unified adapter that composes HTTP transport and SSE event streaming
 * behind a single, environment-aware facade.
 *
 * Usage (from config):
 * ```ts
 * const plugin = new PluginAdapter({ serverUrl: 'http://localhost:8080' })
 * await plugin.callHostFunction('getDocument')
 * plugin.addEventListener('selection', (data) => { ... })
 * ```
 *
 * Usage (from pre-built adapters):
 * ```ts
 * const plugin = new PluginAdapter({ httpAdapter, sseAdapter })
 * ```
 */
export class PluginAdapter {
  private http: HttpAdapter
  private sse: SSEAdapter

  constructor(config: PluginAdapterConfig | PluginAdapterInstances) {
    if ('httpAdapter' in config && 'sseAdapter' in config) {
      // Pre-built adapter instances
      this.http = config.httpAdapter
      this.sse = config.sseAdapter
    } else {
      // Build adapters from serverUrl + optional overrides
      this.http = new HttpAdapter({
        serverUrl: config.serverUrl,
        ...config.http,
      })
      this.sse = new SSEAdapter({
        serverUrl: config.serverUrl,
        ...config.sse,
      })
    }
  }

  // -----------------------------------------------------------------------
  // Environment detection
  // -----------------------------------------------------------------------

  /**
   * Detect the current runtime environment.
   *
   * - `'tauri'`   -- running inside a Tauri webview (`window.__TAURI__` present)
   * - `'cep'`     -- running inside Adobe CEP (`window.__adobe_cep__` present)
   * - `'browser'` -- generic browser / fallback
   */
  static detectEnvironment(): PluginEnvironment {
    if (typeof window === 'undefined') return 'browser'

    // Tauri injects __TAURI__ on the global window object
    if ('__TAURI__' in window) return 'tauri'

    // Adobe CEP injects __adobe_cep__ (and/or CSInterface)
    if ('__adobe_cep__' in window || 'CSInterface' in window) return 'cep'

    return 'browser'
  }

  // -----------------------------------------------------------------------
  // Availability
  // -----------------------------------------------------------------------

  /**
   * Check whether host communication is available.
   * In HTTP mode this always returns `true` -- use `isConnected()` for a
   * network-level health check.
   */
  isAvailable(): boolean {
    return true
  }

  /**
   * Perform a network-level health check against the plugin server.
   *
   * @returns `true` if the server responds
   */
  async isConnected(): Promise<boolean> {
    return this.http.isConnected()
  }

  // -----------------------------------------------------------------------
  // Host function calls (delegates to HttpAdapter)
  // -----------------------------------------------------------------------

  /**
   * Call a host function via the HTTP adapter.
   *
   * @param functionName - Logical function name (e.g. "getDocument")
   * @param args         - Positional arguments forwarded to the endpoint
   * @returns Parsed response body typed as `T`
   */
  async callHostFunction<T = unknown>(functionName: string, ...args: unknown[]): Promise<T> {
    return this.http.callHostFunction<T>(functionName, ...args)
  }

  // -----------------------------------------------------------------------
  // Event subscription (delegates to SSEAdapter)
  // -----------------------------------------------------------------------

  /**
   * Subscribe to a typed SSE event.
   *
   * @param eventType - Event name (e.g. "selection", "document")
   * @param callback  - Strongly-typed callback
   * @returns An unsubscribe function
   */
  addEventListener<T extends EventName>(eventType: T, callback: EventCallback<T>): () => void {
    return this.sse.on(eventType, callback)
  }

  /**
   * Unsubscribe from a typed SSE event.
   *
   * @param eventType - Event name
   * @param callback  - The callback to remove
   */
  removeEventListener<T extends EventName>(eventType: T, callback: EventCallback<T>): void {
    this.sse.off(eventType, callback)
  }

  /**
   * Subscribe to all SSE events with a wildcard callback.
   *
   * @param callback - Receives `{ type, data }` for every event
   * @returns An unsubscribe function
   */
  onAllEvents(callback: WildcardCallback): () => void {
    return this.sse.onAll(callback)
  }

  // -----------------------------------------------------------------------
  // SSE lifecycle
  // -----------------------------------------------------------------------

  /** Connect the SSE stream. */
  connectEvents(): void {
    this.sse.connect()
  }

  /** Disconnect the SSE stream. */
  disconnectEvents(): void {
    this.sse.disconnect()
  }

  /** Whether the SSE stream is currently open. */
  get eventsConnected(): boolean {
    return this.sse.isConnected
  }

  // -----------------------------------------------------------------------
  // Utilities
  // -----------------------------------------------------------------------

  /**
   * Open a URL in the user's default browser.
   * Uses `window.open` as a universal fallback. Consumers in Tauri
   * environments may want to replace this with `tauri.shell.open`.
   *
   * @param url - The URL to open
   */
  openURLInDefaultBrowser(url: string): void {
    if (typeof window !== 'undefined') {
      window.open(url, '_blank')
    }
  }

  // -----------------------------------------------------------------------
  // Adapter access (escape hatches)
  // -----------------------------------------------------------------------

  /** Direct access to the underlying HttpAdapter. */
  getHttpAdapter(): HttpAdapter {
    return this.http
  }

  /** Direct access to the underlying SSEAdapter. */
  getSSEAdapter(): SSEAdapter {
    return this.sse
  }
}

// ---------------------------------------------------------------------------
// Factory helpers
// ---------------------------------------------------------------------------

/**
 * Create a PluginAdapter from a server URL string.
 *
 * @param serverUrl - Base URL for the plugin HTTP server
 * @returns A fully configured PluginAdapter
 */
export function createPluginAdapter(serverUrl: string): PluginAdapter {
  return new PluginAdapter({ serverUrl })
}
