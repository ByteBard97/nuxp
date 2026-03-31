/**
 * SSEAdapter
 *
 * Server-Sent Events adapter for receiving push events from a native plugin
 * server. Merges the typed event system (EventPayloadMap, typed `on<T>()`)
 * with robust reconnection logic and lifecycle management.
 *
 * Self-contained: the server URL is passed in at construction time --
 * no global config or framework imports required.
 */

// ---------------------------------------------------------------------------
// Event Payload Types
// ---------------------------------------------------------------------------

/** Fired when the document selection changes. */
export interface SelectionEvent {
  /** Number of selected items */
  count: number
  /** Array of art handle IDs */
  selectedIds: number[]
}

/** Fired on document lifecycle events (open, close, switch). */
export interface DocumentEvent {
  /** Event type */
  type: 'opened' | 'closed' | 'activated'
  /** Name of the document */
  documentName: string
}

/** Fired when layers are added, removed, or reordered. */
export interface LayersEvent {
  /** Current number of layers */
  layerCount: number
}

/** Fired when art objects are modified. */
export interface ArtChangedEvent {
  /** IDs of changed art objects */
  artIds: number[]
  /** Type of change */
  changeType: 'created' | 'modified' | 'deleted'
}

/** Sent on initial connection with plugin version info. */
export interface VersionEvent {
  /** Plugin version string */
  version: string
  /** Build timestamp */
  build: string
}

// ---------------------------------------------------------------------------
// Event Type Union and Map
// ---------------------------------------------------------------------------

/** Fired when the Flora click tool is used on the artboard. */
export interface ClickEvent {
  /** X coordinate of the click */
  x: number
  /** Y coordinate of the click */
  y: number
  /** Artboard index */
  artboard: number
}

/** Fired when art objects are deleted (e.g. cascade delete of plant entities). */
export interface DeletedEvent {
  /** UUIDs of deleted items */
  uuids: string[]
}

/** Fired when a paste operation is detected. */
export interface PasteEvent {
  /** UUIDs of pasted items */
  uuids: string[]
}

/** Fired when a plant is placed natively by the C++ plugin (zero-roundtrip). */
export interface PlantPlacedEvent {
  /** UUID of the newly placed plant */
  uuid: string
  /** Species ID */
  speciesId: string
  /** Full ref designator (e.g. "TR5") */
  refDesignator: string
  /** Current counter value for the prefix */
  refCounter: number
  /** X coordinate in artboard space */
  x: number
  /** Y coordinate in artboard space */
  y: number
  /** Art bounds */
  bounds: { left: number; top: number; right: number; bottom: number }
  /** Placement duration in milliseconds */
  timing_ms: number
}

/** All recognised event names. */
export type EventName = 'selection' | 'document' | 'layers' | 'artChanged' | 'version' | 'click' | 'deleted' | 'paste' | 'plantPlaced'

/** Maps event names to their payload types. */
export interface EventPayloadMap {
  selection: SelectionEvent
  document: DocumentEvent
  layers: LayersEvent
  artChanged: ArtChangedEvent
  version: VersionEvent
  click: ClickEvent
  deleted: DeletedEvent
  paste: PasteEvent
  plantPlaced: PlantPlacedEvent
}

/** Strongly-typed callback for a specific event. */
export type EventCallback<T extends EventName> = (data: EventPayloadMap[T]) => void

/** Wildcard callback receives the event name together with its data. */
export type WildcardCallback = (event: { type: EventName; data: EventPayloadMap[EventName] }) => void

// ---------------------------------------------------------------------------
// Config
// ---------------------------------------------------------------------------

/** Configuration accepted by the SSEAdapter constructor. */
export interface SSEAdapterConfig {
  /** Base URL for the plugin HTTP server (e.g. "http://localhost:8080") */
  serverUrl: string
  /** Path to the SSE stream endpoint (default: "/events/stream") */
  streamPath?: string
  /** Base reconnection delay in ms (default: 3 000) */
  reconnectDelay?: number
  /** Maximum reconnection attempts before giving up (default: 10) */
  maxReconnectAttempts?: number
}

// ---------------------------------------------------------------------------
// Defaults
// ---------------------------------------------------------------------------

const DEFAULT_STREAM_PATH = '/events/stream'
const DEFAULT_RECONNECT_DELAY = 3_000
const DEFAULT_MAX_RECONNECT_ATTEMPTS = 10

/**
 * Debounce delay for selection events (ms).
 * Illustrator fires kAIArtSelectionChangedNotifier many times during
 * placement, dragging, and tool switches. Without debouncing, every event
 * triggers HTTP calls and store updates across 4+ listeners.
 */
const SELECTION_DEBOUNCE_MS = 150

/** All known event names -- used to register handlers on the EventSource. */
const ALL_EVENT_NAMES: readonly EventName[] = [
  'selection',
  'document',
  'layers',
  'artChanged',
  'version',
  'click',
  'deleted',
  'paste',
  'plantPlaced',
] as const

// ---------------------------------------------------------------------------
// SSEAdapter
// ---------------------------------------------------------------------------

/**
 * SSE client for receiving typed push events from a native plugin server.
 *
 * Usage:
 * ```ts
 * const sse = new SSEAdapter({ serverUrl: 'http://localhost:8080' })
 * sse.on('selection', (data) => console.log(data.count))
 * sse.connect()
 * ```
 */
export class SSEAdapter {
  private serverUrl: string
  private streamPath: string
  private reconnectDelay: number
  private maxReconnectAttempts: number

  private eventSource: EventSource | null = null
  private listeners: Map<string, Set<Function>> = new Map()
  private reconnectAttempts = 0
  private reconnectTimer: ReturnType<typeof setTimeout> | null = null
  private manuallyDisconnected = false
  private selectionDebounceTimer: ReturnType<typeof setTimeout> | null = null
  private pendingSelectionData: EventPayloadMap['selection'] | null = null

  constructor(config: SSEAdapterConfig) {
    this.serverUrl = config.serverUrl
    this.streamPath = config.streamPath ?? DEFAULT_STREAM_PATH
    this.reconnectDelay = config.reconnectDelay ?? DEFAULT_RECONNECT_DELAY
    this.maxReconnectAttempts = config.maxReconnectAttempts ?? DEFAULT_MAX_RECONNECT_ATTEMPTS
  }

  // -----------------------------------------------------------------------
  // Lifecycle
  // -----------------------------------------------------------------------

  /**
   * Open (or re-open) the SSE connection.
   * Safe to call multiple times -- existing connections are cleaned up first.
   */
  connect(): void {
    // Defensive cleanup (e.g. HMR double-mount)
    if (this.eventSource) {
      this.eventSource.close()
      this.eventSource = null
    }

    this.manuallyDisconnected = false
    const url = `${this.serverUrl}${this.streamPath}`

    try {
      this.eventSource = new EventSource(url)

      this.eventSource.onopen = () => {
        this.reconnectAttempts = 0
      }

      this.eventSource.onerror = () => {
        console.error('[SSEAdapter] Connection error')
        this.handleDisconnect()
      }

      // Register a handler for every known event type
      for (const eventType of ALL_EVENT_NAMES) {
        this.registerHandler(eventType)
      }
    } catch (error) {
      console.error('[SSEAdapter] Failed to create EventSource:', error)
      this.handleDisconnect()
    }
  }

  /**
   * Gracefully close the SSE connection and cancel any pending reconnect.
   */
  disconnect(): void {
    this.manuallyDisconnected = true

    if (this.reconnectTimer) {
      clearTimeout(this.reconnectTimer)
      this.reconnectTimer = null
    }

    if (this.selectionDebounceTimer) {
      clearTimeout(this.selectionDebounceTimer)
      this.selectionDebounceTimer = null
      this.pendingSelectionData = null
    }

    if (this.eventSource) {
      this.eventSource.close()
      this.eventSource = null
    }

    this.reconnectAttempts = 0
  }

  // -----------------------------------------------------------------------
  // Subscriptions
  // -----------------------------------------------------------------------

  /**
   * Subscribe to a specific event type with a strongly-typed callback.
   *
   * @returns An unsubscribe function for convenience.
   */
  on<T extends EventName>(event: T, callback: EventCallback<T>): () => void {
    if (!this.listeners.has(event)) {
      this.listeners.set(event, new Set())
    }
    this.listeners.get(event)!.add(callback)

    return () => this.off(event, callback)
  }

  /**
   * Subscribe to all events with a wildcard callback.
   *
   * @returns An unsubscribe function for convenience.
   */
  onAll(callback: WildcardCallback): () => void {
    if (!this.listeners.has('*')) {
      this.listeners.set('*', new Set())
    }
    this.listeners.get('*')!.add(callback)

    return () => {
      this.listeners.get('*')?.delete(callback)
    }
  }

  /**
   * Unsubscribe a previously registered callback.
   */
  off<T extends EventName>(event: T, callback: EventCallback<T>): void {
    this.listeners.get(event)?.delete(callback)
  }

  // -----------------------------------------------------------------------
  // Status
  // -----------------------------------------------------------------------

  /** Whether the underlying EventSource is currently open. */
  get isConnected(): boolean {
    return this.eventSource?.readyState === EventSource.OPEN
  }

  /** Current number of reconnection attempts since last successful open. */
  getReconnectAttempts(): number {
    return this.reconnectAttempts
  }

  /** Get the current server URL. */
  getServerUrl(): string {
    return this.serverUrl
  }

  /**
   * Update the server URL. If already connected, the adapter will
   * disconnect and reconnect automatically.
   */
  setServerUrl(url: string): void {
    const wasConnected = this.isConnected
    this.disconnect()
    this.serverUrl = url
    if (wasConnected) {
      this.connect()
    }
  }

  // -----------------------------------------------------------------------
  // Internal
  // -----------------------------------------------------------------------

  /** Wire up a single event type on the underlying EventSource. */
  private registerHandler(eventType: EventName): void {
    if (!this.eventSource) return

    this.eventSource.addEventListener(eventType, (event: MessageEvent) => {
      try {
        const data = JSON.parse(event.data)

        // Debounce selection events — Illustrator fires many rapid selection
        // changes during placement, dragging, and tool switches. Only dispatch
        // the last one after the burst settles.
        if (eventType === 'selection') {
          this.pendingSelectionData = data as EventPayloadMap['selection']
          if (this.selectionDebounceTimer) {
            clearTimeout(this.selectionDebounceTimer)
          }
          this.selectionDebounceTimer = setTimeout(() => {
            this.selectionDebounceTimer = null
            if (this.pendingSelectionData) {
              this.dispatch('selection', this.pendingSelectionData)
              this.pendingSelectionData = null
            }
          }, SELECTION_DEBOUNCE_MS)
          return
        }

        this.dispatch(eventType, data)
      } catch (error) {
        console.error(`[SSEAdapter] Error parsing ${eventType} event:`, error)
      }
    })
  }

  /** Fan-out an event to matching listeners and wildcard listeners. */
  private dispatch(eventType: EventName, data: EventPayloadMap[EventName]): void {
    // Specific listeners
    this.listeners.get(eventType)?.forEach(cb => {
      try {
        ;(cb as (d: unknown) => void)(data)
      } catch (error) {
        console.error(`[SSEAdapter] Error in ${eventType} listener:`, error)
      }
    })

    // Wildcard listeners
    this.listeners.get('*')?.forEach(cb => {
      try {
        ;(cb as WildcardCallback)({ type: eventType, data })
      } catch (error) {
        console.error('[SSEAdapter] Error in wildcard listener:', error)
      }
    })
  }

  /** Handle unexpected disconnects with exponential back-off reconnection. */
  private handleDisconnect(): void {
    if (this.eventSource) {
      this.eventSource.close()
      this.eventSource = null
    }

    if (this.manuallyDisconnected) return

    if (this.reconnectAttempts < this.maxReconnectAttempts) {
      this.reconnectAttempts++
      // Linear back-off capped at 5x the base delay
      const delay = this.reconnectDelay * Math.min(this.reconnectAttempts, 5)

      console.log(
        `[SSEAdapter] Reconnecting in ${delay}ms (attempt ${this.reconnectAttempts}/${this.maxReconnectAttempts})`,
      )

      this.reconnectTimer = setTimeout(() => {
        this.reconnectTimer = null
        this.connect()
      }, delay)
    } else {
      console.error('[SSEAdapter] Max reconnect attempts reached -- giving up')
    }
  }
}

// ---------------------------------------------------------------------------
// Factory & convenience helpers
// ---------------------------------------------------------------------------

/**
 * Create a new SSEAdapter instance.
 *
 * @param config - SSE configuration (serverUrl required)
 * @returns A new, unconnected SSEAdapter
 */
export function createSSEClient(config: SSEAdapterConfig): SSEAdapter {
  return new SSEAdapter(config)
}

/**
 * Create and immediately connect an SSEAdapter.
 *
 * @param config - SSE configuration (serverUrl required)
 * @returns A connected SSEAdapter
 */
export function connectSSE(config: SSEAdapterConfig): SSEAdapter {
  const client = new SSEAdapter(config)
  client.connect()
  return client
}

/**
 * Disconnect an SSEAdapter instance (null-safe).
 *
 * @param client - The SSEAdapter to disconnect (may be null/undefined)
 */
export function disconnectSSE(client: SSEAdapter | null | undefined): void {
  client?.disconnect()
}
