import { describe, it, expect, beforeEach, vi } from 'vitest'
import { AssetCache } from './SvgCacheService'
import { createSvgLoader, type CachedSvgEntry } from './SvgLoader'

function entry(content: string): CachedSvgEntry {
  return { content, scale: 1, offsetX: 0, offsetY: 0 }
}

describe('createSvgLoader', () => {
  let cache: AssetCache<CachedSvgEntry>
  let fetchFn: ReturnType<typeof vi.fn>

  beforeEach(() => {
    cache = new AssetCache<CachedSvgEntry>()
    fetchFn = vi.fn()
  })

  // -------------------------------------------------------------------------
  // load()
  // -------------------------------------------------------------------------

  it('returns from cache on cache hit (source="cache")', async () => {
    cache.set('icon-1', entry('<svg>cached</svg>'))
    const loader = createSvgLoader(cache, fetchFn)

    const result = await loader.load('icon-1')

    expect(result.success).toBe(true)
    expect(result.source).toBe('cache')
    expect(result.content).toBe('<svg>cached</svg>')
    expect(fetchFn).not.toHaveBeenCalled()
  })

  it('calls fetchFn on cache miss, caches the result, returns source="fetch"', async () => {
    fetchFn.mockResolvedValue(entry('<svg>fetched</svg>'))
    const loader = createSvgLoader(cache, fetchFn)

    const result = await loader.load('icon-2')

    expect(result.success).toBe(true)
    expect(result.source).toBe('fetch')
    expect(result.content).toBe('<svg>fetched</svg>')
    expect(fetchFn).toHaveBeenCalledWith('icon-2')
    // Now it should be in cache
    expect(cache.has('icon-2')).toBe(true)
  })

  it('returns success=false with error when fetchFn returns null', async () => {
    fetchFn.mockResolvedValue(null)
    const loader = createSvgLoader(cache, fetchFn)

    const result = await loader.load('missing')

    expect(result.success).toBe(false)
    expect(result.error).toContain('missing')
    expect(result.source).toBe('fetch')
  })

  it('returns success=false with error when fetchFn throws', async () => {
    fetchFn.mockRejectedValue(new Error('network failure'))
    const loader = createSvgLoader(cache, fetchFn)

    const result = await loader.load('broken')

    expect(result.success).toBe(false)
    expect(result.error).toBe('network failure')
    expect(result.source).toBe('fetch')
  })

  it('returns success=false with stringified error for non-Error throws', async () => {
    fetchFn.mockRejectedValue('string error')
    const loader = createSvgLoader(cache, fetchFn)

    const result = await loader.load('x')

    expect(result.success).toBe(false)
    expect(result.error).toBe('string error')
  })

  // -------------------------------------------------------------------------
  // preload()
  // -------------------------------------------------------------------------

  it('preload() loads multiple keys and reports stats', async () => {
    fetchFn.mockImplementation(async (key: string) => entry(`<svg>${key}</svg>`))
    const loader = createSvgLoader(cache, fetchFn)

    const stats = await loader.preload(['a', 'b', 'c'], 2)

    expect(stats.loaded).toBe(3)
    expect(stats.cached).toBe(0)
    expect(stats.failed).toBe(0)
    expect(cache.has('a')).toBe(true)
    expect(cache.has('b')).toBe(true)
    expect(cache.has('c')).toBe(true)
  })

  it('preload() counts already-cached keys', async () => {
    cache.set('a', entry('<svg>a</svg>'))
    fetchFn.mockResolvedValue(entry('<svg>new</svg>'))
    const loader = createSvgLoader(cache, fetchFn)

    const stats = await loader.preload(['a', 'b'])

    expect(stats.cached).toBe(1)
    expect(stats.loaded).toBe(1)
  })

  it('preload() counts failed fetches', async () => {
    fetchFn.mockResolvedValue(null)
    const loader = createSvgLoader(cache, fetchFn)

    const stats = await loader.preload(['x', 'y'])

    expect(stats.failed).toBe(2)
    expect(stats.loaded).toBe(0)
  })

  it('preload() invokes onProgress callback', async () => {
    fetchFn.mockResolvedValue(entry('<svg/>'))
    const loader = createSvgLoader(cache, fetchFn)
    const progress = vi.fn()

    await loader.preload(['a', 'b'], 5, progress)

    expect(progress).toHaveBeenCalledTimes(2)
    expect(progress).toHaveBeenCalledWith(1, 2)
    expect(progress).toHaveBeenCalledWith(2, 2)
  })

  // -------------------------------------------------------------------------
  // isCached()
  // -------------------------------------------------------------------------

  it('isCached() returns false before load, true after', async () => {
    fetchFn.mockResolvedValue(entry('<svg/>'))
    const loader = createSvgLoader(cache, fetchFn)

    expect(loader.isCached('k')).toBe(false)
    await loader.load('k')
    expect(loader.isCached('k')).toBe(true)
  })
})
