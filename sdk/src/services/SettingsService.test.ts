import { describe, it, expect, beforeEach, vi } from 'vitest'
import { SettingsService } from './SettingsService'

// ---------------------------------------------------------------------------
// Mock localStorage
// ---------------------------------------------------------------------------

function createMockLocalStorage(): Storage {
  let store: Record<string, string> = {}
  return {
    getItem: vi.fn((key: string) => store[key] ?? null),
    setItem: vi.fn((key: string, value: string) => { store[key] = value }),
    removeItem: vi.fn((key: string) => { delete store[key] }),
    clear: vi.fn(() => { store = {} }),
    get length() { return Object.keys(store).length },
    key: vi.fn((i: number) => Object.keys(store)[i] ?? null),
  }
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

interface TestSettings {
  theme: string
  zoom: number
  lastTab?: string
}

describe('SettingsService', () => {
  let storage: Storage

  beforeEach(() => {
    storage = createMockLocalStorage()
    vi.stubGlobal('localStorage', storage)
  })

  it('accepts a storage key and defaults via constructor', () => {
    const svc = new SettingsService<TestSettings>('app-settings', { theme: 'dark', zoom: 1 })
    expect(svc).toBeDefined()
  })

  it('get() returns default value when nothing is stored', () => {
    const svc = new SettingsService<TestSettings>('k', { theme: 'dark', zoom: 1 })
    expect(svc.get('theme')).toBe('dark')
    expect(svc.get('zoom')).toBe(1)
  })

  it('get() returns undefined for unset keys with no default', () => {
    const svc = new SettingsService<TestSettings>('k', {})
    expect(svc.get('lastTab')).toBeUndefined()
  })

  it('set() persists to localStorage and get() retrieves it', () => {
    const svc = new SettingsService<TestSettings>('k', { theme: 'dark' })
    svc.set('theme', 'light')
    expect(storage.setItem).toHaveBeenCalled()
    expect(svc.get('theme')).toBe('light')
  })

  it('getAll() merges defaults with stored values', () => {
    const svc = new SettingsService<TestSettings>('k', { theme: 'dark', zoom: 1 })
    svc.set('zoom', 2)

    const all = svc.getAll()
    expect(all).toEqual({ theme: 'dark', zoom: 2 })
  })

  it('save() merges partial updates into existing stored values', () => {
    const svc = new SettingsService<TestSettings>('k', { theme: 'dark', zoom: 1 })
    svc.set('theme', 'light')
    svc.save({ zoom: 3 })

    const all = svc.getAll()
    expect(all.theme).toBe('light')
    expect(all.zoom).toBe(3)
  })

  it('reset() clears localStorage so defaults are returned again', () => {
    const svc = new SettingsService<TestSettings>('k', { theme: 'dark', zoom: 1 })
    svc.set('theme', 'light')
    svc.reset()

    expect(storage.removeItem).toHaveBeenCalledWith('k')
    expect(svc.get('theme')).toBe('dark')
  })

  it('handles JSON parse errors gracefully (corrupted localStorage)', () => {
    const consoleSpy = vi.spyOn(console, 'error').mockImplementation(() => {})
    // Inject corrupt data directly
    storage.getItem = vi.fn(() => '{not valid json!!!}')

    const svc = new SettingsService<TestSettings>('k', { theme: 'dark', zoom: 1 })
    const all = svc.getAll()

    // Should fall back to defaults
    expect(all).toEqual({ theme: 'dark', zoom: 1 })
    expect(consoleSpy).toHaveBeenCalled()
    consoleSpy.mockRestore()
  })

  it('set() works when localStorage is initially empty', () => {
    const svc = new SettingsService<TestSettings>('k', {})
    svc.set('lastTab', 'overview')
    expect(svc.get('lastTab')).toBe('overview')
  })

  it('defaults can be omitted (empty object fallback)', () => {
    const svc = new SettingsService<TestSettings>('k')
    expect(svc.getAll()).toEqual({})
  })
})
