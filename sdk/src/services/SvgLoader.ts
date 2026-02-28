/**
 * SvgLoader
 *
 * Generic SVG asset loading with a cache-first strategy.
 *
 * Strategy:
 * 1. Check the in-memory AssetCache first (instant).
 * 2. If cached, return immediately.
 * 3. If not cached, delegate to a caller-supplied fetch function.
 * 4. Store the result in the cache for future use.
 * 5. Return the SVG content.
 *
 * This module is a standalone utility -- it does NOT depend on the bridge.
 * It is parameterised over a fetch function so that callers can supply their
 * own network/filesystem backend.
 */

import { AssetCache } from './SvgCacheService'

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

/** Result of loading an SVG asset. */
export interface SvgLoadResult {
  success: boolean
  /** Raw SVG string content (undefined on failure) */
  content?: string
  /** Where the content was sourced from */
  source: 'cache' | 'fetch'
  /** Uniform scale factor for the SVG (1.0 = no scaling) */
  scale: number
  /** Horizontal offset hint */
  offsetX: number
  /** Vertical offset hint */
  offsetY: number
  /** Error message on failure */
  error?: string
}

/** Shape of the object stored in the cache for each SVG asset. */
export interface CachedSvgEntry {
  content: string
  scale: number
  offsetX: number
  offsetY: number
}

/**
 * A function that fetches SVG content by key.
 *
 * Implementations might hit a REST API, read from the local filesystem,
 * decompress an SVGZ archive, etc.  Returning `null` signals that the
 * asset is not available.
 */
export type SvgFetchFn = (key: string) => Promise<CachedSvgEntry | null>

// ---------------------------------------------------------------------------
// Loader
// ---------------------------------------------------------------------------

/**
 * Create an SvgLoader bound to a given cache and fetch function.
 *
 * The loader is intentionally a plain object (not a class) because it
 * carries no mutable state beyond the cache reference.
 */
export function createSvgLoader(cache: AssetCache<CachedSvgEntry>, fetchFn: SvgFetchFn) {
  /**
   * Load an SVG by key using cache-first strategy.
   *
   * @param key - Unique identifier for the asset (e.g. a numeric ID, a
   *              path, or any string the fetch function understands).
   */
  async function load(key: string): Promise<SvgLoadResult> {
    // 1. Cache hit
    const cached = cache.get(key)
    if (cached) {
      return {
        success: true,
        content: cached.content,
        source: 'cache',
        scale: cached.scale,
        offsetX: cached.offsetX,
        offsetY: cached.offsetY,
      }
    }

    // 2. Cache miss -- delegate to fetch function
    try {
      const fetched = await fetchFn(key)

      if (fetched) {
        cache.set(key, fetched)
        return {
          success: true,
          content: fetched.content,
          source: 'fetch',
          scale: fetched.scale,
          offsetX: fetched.offsetX,
          offsetY: fetched.offsetY,
        }
      }

      return {
        success: false,
        source: 'fetch',
        scale: 1,
        offsetX: 0,
        offsetY: 0,
        error: `No SVG available for key "${key}"`,
      }
    } catch (err) {
      const message = err instanceof Error ? err.message : String(err)
      return {
        success: false,
        source: 'fetch',
        scale: 1,
        offsetX: 0,
        offsetY: 0,
        error: message,
      }
    }
  }

  /**
   * Preload SVGs for multiple keys in the background.
   *
   * Useful for warming the cache when displaying a list of assets.
   *
   * @param keys        - Asset keys to preload.
   * @param concurrency - Maximum number of parallel fetches (default 5).
   * @param onProgress  - Optional callback invoked after each key is processed.
   */
  async function preload(
    keys: string[],
    concurrency = 5,
    onProgress?: (loaded: number, total: number) => void
  ): Promise<{ loaded: number; cached: number; failed: number }> {
    const stats = { loaded: 0, cached: 0, failed: 0 }
    let processed = 0

    const queue = [...keys]

    async function worker(): Promise<void> {
      while (queue.length > 0) {
        const key = queue.shift()
        if (!key) break

        if (cache.has(key)) {
          stats.cached++
        } else {
          const result = await load(key)
          if (result.success) {
            stats.loaded++
          } else {
            stats.failed++
          }
        }

        processed++
        onProgress?.(processed, keys.length)
      }
    }

    const workers = Array(Math.min(concurrency, keys.length))
      .fill(null)
      .map(() => worker())

    await Promise.all(workers)
    return stats
  }

  /**
   * Check whether an SVG is already cached for the given key.
   */
  function isCached(key: string): boolean {
    return cache.has(key)
  }

  return { load, preload, isCached }
}

/** Convenience type for the object returned by `createSvgLoader`. */
export type SvgLoader = ReturnType<typeof createSvgLoader>
