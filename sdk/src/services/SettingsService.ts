/**
 * Generic Settings Service
 *
 * Persists user settings to localStorage with type-safe access.
 * The settings shape is defined by the consuming application via the
 * generic type parameter.
 *
 * @example
 * ```ts
 * interface MySettings {
 *   theme: string
 *   zoom: number
 *   lastActiveTab?: string
 * }
 *
 * const defaults: Partial<MySettings> = { theme: 'dark', zoom: 1 }
 * const settings = new SettingsService<MySettings>('my-app-settings', defaults)
 *
 * settings.set('theme', 'light')
 * const theme = settings.get('theme') // 'light'
 * ```
 */

/**
 * A generic, localStorage-backed settings service.
 *
 * @typeParam T - The settings shape, must be a plain object type.
 */
export class SettingsService<T extends Record<string, unknown>> {
  private readonly storageKey: string
  private readonly defaults: Partial<T>

  /**
   * Create a new SettingsService.
   *
   * @param storageKey - The localStorage key used for persistence.
   * @param defaults - Default values for settings fields. Used when a field
   *   has not been explicitly set.
   */
  constructor(storageKey: string, defaults: Partial<T> = {}) {
    this.storageKey = storageKey
    this.defaults = defaults
  }

  /**
   * Load all persisted settings from localStorage, merged with defaults.
   *
   * @returns The full settings object (defaults + stored overrides).
   */
  getAll(): Partial<T> {
    try {
      const stored = localStorage.getItem(this.storageKey)
      const parsed: Partial<T> = stored ? JSON.parse(stored) : {}
      return { ...this.defaults, ...parsed }
    } catch (error) {
      console.error(`[SettingsService] Failed to load settings from "${this.storageKey}":`, error)
      return { ...this.defaults }
    }
  }

  /**
   * Get a specific setting value.
   *
   * @param key - The setting key to retrieve.
   * @returns The stored value, the default value, or undefined.
   */
  get<K extends keyof T>(key: K): T[K] | undefined {
    const all = this.getAll()
    return all[key] as T[K] | undefined
  }

  /**
   * Set a specific setting value and persist to localStorage.
   *
   * @param key - The setting key to update.
   * @param value - The new value.
   */
  set<K extends keyof T>(key: K, value: T[K]): void {
    this.save({ [key]: value } as unknown as Partial<T>)
  }

  /**
   * Merge partial settings into the stored values and persist.
   *
   * @param settings - A partial settings object to merge in.
   */
  save(settings: Partial<T>): void {
    try {
      const existing = this.loadRaw()
      const merged = { ...existing, ...settings }
      localStorage.setItem(this.storageKey, JSON.stringify(merged))
    } catch (error) {
      console.error(`[SettingsService] Failed to save settings to "${this.storageKey}":`, error)
    }
  }

  /**
   * Reset all settings to defaults by clearing localStorage.
   */
  reset(): void {
    try {
      localStorage.removeItem(this.storageKey)
    } catch (error) {
      console.error(`[SettingsService] Failed to clear settings from "${this.storageKey}":`, error)
    }
  }

  /**
   * Load raw stored values (without defaults merged in).
   */
  private loadRaw(): Partial<T> {
    try {
      const stored = localStorage.getItem(this.storageKey)
      return stored ? JSON.parse(stored) : {}
    } catch {
      return {}
    }
  }
}
