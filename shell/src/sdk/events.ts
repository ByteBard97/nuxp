/**
 * NUXP Event System
 *
 * Provides a typed event emitter for subscribing to Illustrator events
 * pushed from the C++ plugin via the /events endpoint.
 *
 * @example
 * ```typescript
 * import { events } from '@/sdk';
 *
 * // Subscribe to selection changes
 * const unsubscribe = events.on('selectionChanged', (data) => {
 *   console.log('Selection changed:', data);
 * });
 *
 * // Subscribe to all events
 * events.on('*', (event) => {
 *   console.log('Event:', event.type, event.data);
 * });
 *
 * // Clean up when done
 * unsubscribe();
 * ```
 */

/**
 * Callback function type for event handlers
 */
export type EventCallback = (data: any) => void;

/**
 * Known event types from the Illustrator plugin
 */
export type EventType =
  | 'selectionChanged'
  | 'documentChanged'
  | 'artChanged'
  | 'layersChanged'
  | 'connected'
  | 'disconnected'
  | '*';

/**
 * Structure of events received from the plugin
 */
export interface PluginEvent {
  /** Event type identifier */
  type: string;
  /** Event payload data */
  data?: any;
  /** Timestamp when event was generated */
  timestamp?: number;
}

/**
 * Internal EventEmitter class
 *
 * Handles subscription management and event dispatch with support
 * for wildcard listeners that receive all events.
 */
class EventEmitter {
  private listeners: Map<string, Set<EventCallback>> = new Map();

  /**
   * Subscribe to an event
   *
   * @param event - The event type to listen for (or '*' for all events)
   * @param callback - Function to call when the event occurs
   * @returns Unsubscribe function to remove the listener
   */
  on(event: EventType | string, callback: EventCallback): () => void {
    if (!this.listeners.has(event)) {
      this.listeners.set(event, new Set());
    }
    this.listeners.get(event)!.add(callback);

    // Return unsubscribe function
    return () => this.off(event, callback);
  }

  /**
   * Unsubscribe from an event
   *
   * @param event - The event type to unsubscribe from
   * @param callback - The callback function to remove
   */
  off(event: EventType | string, callback: EventCallback): void {
    this.listeners.get(event)?.delete(callback);
  }

  /**
   * Emit an event to all subscribed listeners
   *
   * Calls both specific event listeners and wildcard ('*') listeners.
   * Wildcard listeners receive an object with { type, data } structure.
   *
   * @param event - The event type being emitted
   * @param data - Optional data payload for the event
   */
  emit(event: string, data?: any): void {
    // Call specific event listeners
    const callbacks = this.listeners.get(event);
    if (callbacks) {
      callbacks.forEach((cb) => {
        try {
          cb(data);
        } catch (e) {
          console.error(`[EventEmitter] Error in listener for ${event}:`, e);
        }
      });
    }

    // Call wildcard listeners with event object
    const wildcardCallbacks = this.listeners.get('*');
    if (wildcardCallbacks) {
      wildcardCallbacks.forEach((cb) => {
        try {
          cb({ type: event, data });
        } catch (e) {
          console.error(`[EventEmitter] Error in wildcard listener:`, e);
        }
      });
    }
  }

  /**
   * Clear all event listeners
   *
   * Useful for cleanup during testing or component unmount
   */
  clear(): void {
    this.listeners.clear();
  }

  /**
   * Get the count of listeners for an event
   *
   * @param event - Event type to count listeners for
   * @returns Number of registered listeners
   */
  listenerCount(event: string): number {
    return this.listeners.get(event)?.size ?? 0;
  }
}

/**
 * Global event emitter instance
 *
 * This is the internal emitter used by the events API.
 * Prefer using the `events` object for public access.
 */
export const eventBus = new EventEmitter();

/**
 * Public Events API
 *
 * Provides a clean interface for subscribing to plugin events.
 * Internal emit functionality is exposed via _emit for use by the bridge.
 */
export const events = {
  /**
   * Subscribe to an event
   *
   * @param event - Event type to listen for (or '*' for all events)
   * @param callback - Function called when event occurs
   * @returns Unsubscribe function
   */
  on: (event: EventType | string, callback: EventCallback): (() => void) =>
    eventBus.on(event, callback),

  /**
   * Unsubscribe from an event
   *
   * @param event - Event type to unsubscribe from
   * @param callback - The callback to remove
   */
  off: (event: EventType | string, callback: EventCallback): void =>
    eventBus.off(event, callback),

  /**
   * Clear all event listeners
   */
  clear: (): void => eventBus.clear(),

  /**
   * Internal: Emit an event (used by the bridge event loop)
   *
   * @param event - Event type to emit
   * @param data - Event data payload
   * @internal
   */
  _emit: (event: string, data?: any): void => eventBus.emit(event, data),
};
