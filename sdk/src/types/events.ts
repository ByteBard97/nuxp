/**
 * SSE Event Types
 *
 * Type definitions for Server-Sent Events from the C++ Illustrator plugin.
 * Events are received directly via SSE without any translation layer.
 */

/**
 * Click event from a custom click tool in Illustrator.
 */
export interface ClickEvent {
  /** X coordinate in artboard space */
  x: number
  /** Y coordinate in artboard space */
  y: number
  /** Index of the artboard where the click occurred */
  artboardIndex: number
}

/**
 * Selection change event when the user selects items in Illustrator.
 */
export interface SelectionEvent {
  /** Number of selected items */
  count: number
  /** UUIDs of the selected art items */
  uuids: string[]
}

/**
 * Deletion event when tracked art items are removed from the document.
 */
export interface DeletedEvent {
  /** UUIDs of the deleted art items */
  uuids: string[]
}

/**
 * Paste/reassign event when art items are copy-pasted.
 * Maps an old UUID to a newly generated UUID for the pasted copy.
 */
export interface PasteEvent {
  /** UUID of the original art item */
  oldUuid: string
  /** UUID assigned to the pasted copy */
  newUuid: string
}

/**
 * Document lifecycle event fired when a document is opened, closed, etc.
 */
export interface DocumentEvent {
  /** The lifecycle action that occurred */
  event: 'activated' | 'opened' | 'closed' | 'created'
  /** Document name */
  name: string
  /** Document file path */
  path: string
}

/**
 * Version information reported by the C++ plugin on connection.
 */
export interface VersionEvent {
  /** Semantic version string */
  version: string
  /** Build identifier */
  build: string
  /** Build date (optional) */
  buildDate?: string
  /** Build time (optional) */
  buildTime?: string
}

/**
 * SSE event type string literals.
 */
export type SSEEventType = 'click' | 'selection' | 'deleted' | 'paste' | 'document' | 'version'

/**
 * Union type for all SSE event payloads.
 */
export type SSEEventPayload =
  | ClickEvent
  | SelectionEvent
  | DeletedEvent
  | PasteEvent
  | DocumentEvent
  | VersionEvent

/**
 * Type-safe mapping from event type string to its payload interface.
 */
export interface SSEEventMap {
  click: ClickEvent
  selection: SelectionEvent
  deleted: DeletedEvent
  paste: PasteEvent
  document: DocumentEvent
  version: VersionEvent
}
