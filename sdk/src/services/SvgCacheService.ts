/**
 * AssetCache<T>
 *
 * Generic in-memory cache with optional TTL-based expiration and
 * configurable maximum size (entry count).
 *
 * Although originally extracted from an SVG-specific cache, this
 * implementation is fully generic -- it can cache any value type.
 */

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

/** Options for configuring an AssetCache instance. */
export interface AssetCacheOptions {
  /**
   * Maximum number of entries the cache will hold.
   * When the limit is reached the oldest entry (by insertion time) is evicted.
   * Set to 0 or omit for unlimited entries.
   */
  maxEntries?: number

  /**
   * Default time-to-live in milliseconds for cache entries.
   * Entries older than this are considered stale and will not be returned
   * by `get()`.  Set to 0 or omit to disable TTL expiration.
   */
  ttlMs?: number
}

/** Internal wrapper around a cached value. */
interface CacheEntry<T> {
  value: T
  /** Timestamp (ms since epoch) when the entry was stored. */
  storedAt: number
}

/** Snapshot of cache statistics. */
export interface CacheStats {
  /** Number of entries currently in the cache. */
  entryCount: number
  /** Sum of all entry ages in milliseconds (useful for monitoring). */
  totalAgeMs: number
}

// ---------------------------------------------------------------------------
// Implementation
// ---------------------------------------------------------------------------

export class AssetCache<T> {
  private entries = new Map<string, CacheEntry<T>>()
  private maxEntries: number
  private ttlMs: number

  constructor(options: AssetCacheOptions = {}) {
    this.maxEntries = options.maxEntries ?? 0
    this.ttlMs = options.ttlMs ?? 0
  }

  // -----------------------------------------------------------------------
  // Public API
  // -----------------------------------------------------------------------

  /**
   * Retrieve a cached value by key.
   *
   * Returns `undefined` if the key is not present or the entry has expired.
   */
  get(key: string): T | undefined {
    const entry = this.entries.get(key)
    if (!entry) return undefined

    // TTL check
    if (this.ttlMs > 0 && Date.now() - entry.storedAt > this.ttlMs) {
      this.entries.delete(key)
      return undefined
    }

    return entry.value
  }

  /**
   * Store a value in the cache under the given key.
   *
   * If the cache is full the oldest entry is evicted first.
   */
  set(key: string, value: T): void {
    // Evict oldest if at capacity (and key is not already present)
    if (
      this.maxEntries > 0 &&
      this.entries.size >= this.maxEntries &&
      !this.entries.has(key)
    ) {
      this.evictOldest()
    }

    this.entries.set(key, {
      value,
      storedAt: Date.now(),
    })
  }

  /**
   * Check whether a non-expired entry exists for the key.
   */
  has(key: string): boolean {
    return this.get(key) !== undefined
  }

  /**
   * Remove a single entry from the cache.
   *
   * @returns `true` if the entry existed and was removed.
   */
  delete(key: string): boolean {
    return this.entries.delete(key)
  }

  /**
   * Remove all entries from the cache.
   */
  clear(): void {
    this.entries.clear()
  }

  /**
   * Return a snapshot of the current cache statistics.
   */
  getStats(): CacheStats {
    const now = Date.now()
    let totalAgeMs = 0
    for (const entry of this.entries.values()) {
      totalAgeMs += now - entry.storedAt
    }

    return {
      entryCount: this.entries.size,
      totalAgeMs,
    }
  }

  /**
   * Return all keys currently in the cache (including potentially expired
   * entries -- use `has()` if freshness matters).
   */
  keys(): string[] {
    return [...this.entries.keys()]
  }

  /**
   * Number of entries currently stored (including potentially expired ones).
   */
  get size(): number {
    return this.entries.size
  }

  // -----------------------------------------------------------------------
  // Internals
  // -----------------------------------------------------------------------

  /** Evict the oldest entry by insertion time. */
  private evictOldest(): void {
    let oldestKey: string | null = null
    let oldestTime = Infinity

    for (const [key, entry] of this.entries) {
      if (entry.storedAt < oldestTime) {
        oldestTime = entry.storedAt
        oldestKey = key
      }
    }

    if (oldestKey !== null) {
      this.entries.delete(oldestKey)
    }
  }
}

// ---------------------------------------------------------------------------
// Factory helper
// ---------------------------------------------------------------------------

/**
 * Create an AssetCache instance with the given options.
 *
 * This is a convenience wrapper around `new AssetCache(options)` for
 * contexts where a factory function reads more naturally than a constructor.
 */
export function createAssetCache<T>(options?: AssetCacheOptions): AssetCache<T> {
  return new AssetCache<T>(options)
}
