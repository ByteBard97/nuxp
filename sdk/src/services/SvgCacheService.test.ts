import { describe, it, expect, beforeEach, vi, afterEach } from 'vitest'
import { AssetCache, createAssetCache } from './SvgCacheService'

describe('AssetCache', () => {
  let cache: AssetCache<string>

  beforeEach(() => {
    vi.useFakeTimers()
    cache = new AssetCache<string>()
  })

  afterEach(() => {
    vi.useRealTimers()
  })

  // -------------------------------------------------------------------------
  // Basic CRUD
  // -------------------------------------------------------------------------

  it('set/get stores and retrieves a value', () => {
    cache.set('a', 'alpha')
    expect(cache.get('a')).toBe('alpha')
  })

  it('get returns undefined for missing key', () => {
    expect(cache.get('nope')).toBeUndefined()
  })

  it('has() returns true for existing key, false for missing', () => {
    cache.set('a', 'alpha')
    expect(cache.has('a')).toBe(true)
    expect(cache.has('b')).toBe(false)
  })

  it('delete() removes an entry and returns true', () => {
    cache.set('a', 'alpha')
    expect(cache.delete('a')).toBe(true)
    expect(cache.get('a')).toBeUndefined()
  })

  it('delete() returns false for non-existent key', () => {
    expect(cache.delete('nope')).toBe(false)
  })

  it('clear() removes all entries', () => {
    cache.set('a', 'alpha')
    cache.set('b', 'beta')
    cache.clear()
    expect(cache.size).toBe(0)
    expect(cache.get('a')).toBeUndefined()
  })

  // -------------------------------------------------------------------------
  // TTL expiration
  // -------------------------------------------------------------------------

  it('returns value before TTL expires', () => {
    const ttlCache = new AssetCache<string>({ ttlMs: 1000 })
    ttlCache.set('a', 'alpha')
    vi.advanceTimersByTime(500)
    expect(ttlCache.get('a')).toBe('alpha')
  })

  it('returns undefined after TTL expires and removes the entry', () => {
    const ttlCache = new AssetCache<string>({ ttlMs: 1000 })
    ttlCache.set('a', 'alpha')
    vi.advanceTimersByTime(1001)
    expect(ttlCache.get('a')).toBeUndefined()
  })

  it('has() returns false for expired entries', () => {
    const ttlCache = new AssetCache<string>({ ttlMs: 500 })
    ttlCache.set('a', 'alpha')
    vi.advanceTimersByTime(501)
    expect(ttlCache.has('a')).toBe(false)
  })

  // -------------------------------------------------------------------------
  // Max entries eviction
  // -------------------------------------------------------------------------

  it('evicts oldest entry when maxEntries is exceeded', () => {
    const bounded = new AssetCache<string>({ maxEntries: 2 })
    bounded.set('a', 'alpha')
    vi.advanceTimersByTime(1)
    bounded.set('b', 'beta')
    vi.advanceTimersByTime(1)
    bounded.set('c', 'charlie')

    // 'a' should have been evicted
    expect(bounded.get('a')).toBeUndefined()
    expect(bounded.get('b')).toBe('beta')
    expect(bounded.get('c')).toBe('charlie')
    expect(bounded.size).toBe(2)
  })

  it('does not evict when updating an existing key', () => {
    const bounded = new AssetCache<string>({ maxEntries: 2 })
    bounded.set('a', 'alpha')
    bounded.set('b', 'beta')
    bounded.set('a', 'alpha2') // update, not new entry
    expect(bounded.size).toBe(2)
    expect(bounded.get('a')).toBe('alpha2')
    expect(bounded.get('b')).toBe('beta')
  })

  // -------------------------------------------------------------------------
  // Stats, keys, size
  // -------------------------------------------------------------------------

  it('getStats() returns correct entryCount', () => {
    cache.set('a', 'alpha')
    cache.set('b', 'beta')
    const stats = cache.getStats()
    expect(stats.entryCount).toBe(2)
  })

  it('getStats() returns correct totalAgeMs', () => {
    cache.set('a', 'alpha')
    vi.advanceTimersByTime(100)
    cache.set('b', 'beta')
    vi.advanceTimersByTime(50)

    const stats = cache.getStats()
    // 'a' is 150ms old, 'b' is 50ms old => total 200ms
    expect(stats.totalAgeMs).toBe(200)
  })

  it('keys() returns all stored keys', () => {
    cache.set('x', '1')
    cache.set('y', '2')
    cache.set('z', '3')
    expect(cache.keys()).toEqual(['x', 'y', 'z'])
  })

  it('size property reflects current entry count', () => {
    expect(cache.size).toBe(0)
    cache.set('a', 'alpha')
    expect(cache.size).toBe(1)
    cache.delete('a')
    expect(cache.size).toBe(0)
  })

  // -------------------------------------------------------------------------
  // Factory
  // -------------------------------------------------------------------------

  it('createAssetCache factory produces a working instance', () => {
    const c = createAssetCache<number>({ maxEntries: 10 })
    c.set('n', 42)
    expect(c.get('n')).toBe(42)
    expect(c).toBeInstanceOf(AssetCache)
  })
})
