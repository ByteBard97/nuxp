/**
 * Document Index Service
 *
 * A framework-agnostic service for reading and writing a typed index of
 * items stored in an Illustrator document's XMP metadata via bridge calls.
 *
 * A plain TypeScript class that applications can wrap in whatever state
 * management they prefer. No framework dependencies.
 *
 * @typeParam T - The item type stored in the index. Must have at minimum
 *   an `id` field of type `string`.
 *
 * @example
 * ```ts
 * interface Widget { id: string; name: string; x: number; y: number }
 *
 * const index = new DocumentIndexService<Widget>(bridge.callHost)
 * await index.readIndex()
 * index.addItem({ id: 'w1', name: 'Button', x: 100, y: 200 })
 * await index.writeIndex()
 * ```
 */

import type { BridgeCallFn } from '../primitives/types'

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

/** Constraint for items stored in the document index. */
export interface Identifiable {
  id: string
}

/**
 * The serialized shape of a document index stored in XMP metadata.
 *
 * @typeParam T - The item type.
 */
export interface DocumentIndex<T extends Identifiable> {
  /** Schema version for migration support */
  schemaVersion: number
  /** ISO-8601 creation timestamp */
  createdAt: string
  /** ISO-8601 last-update timestamp */
  updatedAt: string
  /** Map of item ID to item data */
  items: Record<string, T>
  /** Optional application-specific metadata */
  metadata?: Record<string, unknown>
}

/**
 * Options for constructing a DocumentIndexService.
 */
export interface DocumentIndexOptions {
  /**
   * The bridge function name used to read the index from XMP metadata.
   * Defaults to `'readIndexFromXMP'`.
   */
  readFunctionName?: string
  /**
   * The bridge function name used to write the index to XMP metadata.
   * Defaults to `'writeIndexToXMP'`.
   */
  writeFunctionName?: string
  /**
   * Schema version written to new indexes.
   * Defaults to `1`.
   */
  schemaVersion?: number
}

// ---------------------------------------------------------------------------
// Service
// ---------------------------------------------------------------------------

/**
 * Manages a typed item index persisted to Illustrator document XMP metadata.
 *
 * @typeParam T - Item type, must extend `Identifiable` (has an `id: string`).
 */
export class DocumentIndexService<T extends Identifiable> {
  private bridge: BridgeCallFn
  private index: DocumentIndex<T>
  private isDirty = false
  private isLoading = false
  private lastSyncTime: Date | null = null

  private readonly readFn: string
  private readonly writeFn: string
  private readonly schemaVersion: number

  /**
   * Create a new DocumentIndexService.
   *
   * @param bridge - The bridge call function (e.g. `hostBridge.callHost`).
   * @param options - Optional configuration overrides.
   */
  constructor(bridge: BridgeCallFn, options: DocumentIndexOptions = {}) {
    this.bridge = bridge
    this.readFn = options.readFunctionName ?? 'readIndexFromXMP'
    this.writeFn = options.writeFunctionName ?? 'writeIndexToXMP'
    this.schemaVersion = options.schemaVersion ?? 1

    this.index = this.createEmptyIndex()
  }

  // ---------------------------------------------------------------------------
  // Read / Write
  // ---------------------------------------------------------------------------

  /**
   * Read the index from document XMP metadata via bridge.
   *
   * @returns The loaded index, or `null` if no index exists or on error.
   */
  async readIndex(): Promise<DocumentIndex<T> | null> {
    this.isLoading = true
    try {
      const result = await this.bridge<unknown>(this.readFn)
      const parsed = this.parseResult(result)

      if (!parsed) return null

      if (parsed.error) {
        const isExpected =
          parsed.error.includes('no_index') ||
          parsed.error.includes('not_found') ||
          parsed.error.includes('No index') ||
          parsed.error.includes('No active document')

        if (isExpected) {
          return null
        }

        console.error('[DocumentIndexService] Error reading index from XMP:', parsed.error)
        return null
      }

      if (!parsed.data) return null

      const indexData: DocumentIndex<T> =
        typeof parsed.data === 'string' ? JSON.parse(parsed.data) : parsed.data

      this.index = indexData
      this.lastSyncTime = new Date()
      this.isDirty = false

      return indexData
    } catch (error) {
      console.error('[DocumentIndexService] Error reading index:', error)
      return null
    } finally {
      this.isLoading = false
    }
  }

  /**
   * Write the current in-memory index to document XMP metadata via bridge.
   *
   * @returns `true` on success, `false` on failure.
   */
  async writeIndex(): Promise<boolean> {
    this.isLoading = true
    try {
      this.index.updatedAt = new Date().toISOString()
      const jsonString = JSON.stringify(this.index)
      const result = await this.bridge<unknown>(this.writeFn, jsonString)
      const parsed = this.parseResult(result)

      if (parsed?.error) {
        console.error('[DocumentIndexService] Error writing index:', parsed.error)
        return false
      }

      if (!parsed?.success) {
        console.error('[DocumentIndexService] Failed to write index')
        return false
      }

      this.lastSyncTime = new Date()
      this.isDirty = false
      return true
    } catch (error) {
      console.error('[DocumentIndexService] Error writing index:', error)
      return false
    } finally {
      this.isLoading = false
    }
  }

  // ---------------------------------------------------------------------------
  // CRUD
  // ---------------------------------------------------------------------------

  /**
   * Add an item to the index (in memory). Call `writeIndex()` to persist.
   *
   * @param item - The item to add. Its `id` is used as the key.
   */
  addItem(item: T): void {
    this.index.items[item.id] = item
    this.index.updatedAt = new Date().toISOString()
    this.isDirty = true
  }

  /**
   * Update an existing item by merging partial fields.
   *
   * @param id - The item ID.
   * @param updates - Partial fields to merge into the existing item.
   */
  updateItem(id: string, updates: Partial<T>): void {
    const item = this.index.items[id]
    if (!item) {
      console.warn(`[DocumentIndexService] Cannot update item "${id}" -- not found`)
      return
    }
    Object.assign(item, updates)
    this.index.updatedAt = new Date().toISOString()
    this.isDirty = true
  }

  /**
   * Remove an item from the index by ID.
   *
   * @param id - The item ID to remove.
   */
  removeItem(id: string): void {
    if (!this.index.items[id]) {
      console.warn(`[DocumentIndexService] Cannot remove item "${id}" -- not found`)
      return
    }
    delete this.index.items[id]
    this.index.updatedAt = new Date().toISOString()
    this.isDirty = true
  }

  /**
   * Get a single item by ID.
   *
   * @param id - The item ID.
   * @returns The item, or `undefined` if not found.
   */
  getItem(id: string): T | undefined {
    return this.index.items[id]
  }

  /**
   * Get all items as an array.
   */
  getAllItems(): T[] {
    return Object.values(this.index.items)
  }

  /**
   * Get the count of items in the index.
   */
  getItemCount(): number {
    return Object.keys(this.index.items).length
  }

  /**
   * Clear the entire in-memory index and reset to empty state.
   */
  clear(): void {
    this.index = this.createEmptyIndex()
    this.lastSyncTime = null
    this.isDirty = false
  }

  // ---------------------------------------------------------------------------
  // State queries
  // ---------------------------------------------------------------------------

  /** Whether the in-memory index has unsaved changes. */
  needsSave(): boolean {
    return this.isDirty
  }

  /** Whether a read/write operation is in progress. */
  loading(): boolean {
    return this.isLoading
  }

  /** The time of the last successful read or write, or `null`. */
  getLastSyncTime(): Date | null {
    return this.lastSyncTime
  }

  /** Get the raw index object (read-only snapshot). */
  getIndex(): Readonly<DocumentIndex<T>> {
    return this.index
  }

  /**
   * Set or retrieve application-specific metadata on the index.
   *
   * @param key - Metadata key.
   * @param value - If provided, sets the value. If omitted, returns current value.
   */
  metadata(key: string, value?: unknown): unknown {
    if (!this.index.metadata) {
      this.index.metadata = {}
    }
    if (value !== undefined) {
      this.index.metadata[key] = value
      this.index.updatedAt = new Date().toISOString()
      this.isDirty = true
    }
    return this.index.metadata[key]
  }

  // ---------------------------------------------------------------------------
  // Private helpers
  // ---------------------------------------------------------------------------

  private createEmptyIndex(): DocumentIndex<T> {
    return {
      schemaVersion: this.schemaVersion,
      createdAt: new Date().toISOString(),
      updatedAt: new Date().toISOString(),
      items: {} as Record<string, T>,
    }
  }

  /**
   * Parse a bridge result that may be a JSON string or already an object.
   */
  private parseResult(result: unknown): Record<string, any> | null {
    if (result == null) return null

    if (typeof result === 'string') {
      try {
        return JSON.parse(result)
      } catch {
        console.error('[DocumentIndexService] Failed to parse bridge result:', result)
        return null
      }
    }

    if (typeof result === 'object') {
      return result as Record<string, any>
    }

    return null
  }
}
