/**
 * Appearance Configuration Service
 *
 * Manages theme presets, color overrides, font application, background
 * settings, and opacity -- all driven by the consuming application's
 * preset definitions.
 *
 * No built-in presets are shipped. The app provides:
 * - A default preset
 * - A registry of available presets (or a lookup function)
 * - Optional storage key prefix for multi-app isolation
 *
 * @example
 * ```ts
 * import type { AppearancePreset } from '../types/appearance'
 *
 * const presets: AppearancePreset[] = [darkPreset, lightPreset]
 * const service = new AppearanceConfigService({
 *   defaultPreset: darkPreset,
 *   presets,
 *   storagePrefix: 'myapp',
 * })
 * service.initialize()
 * ```
 */

import type {
  AppearancePreset,
  AppearanceOverrides,
  AppearanceColors,
  AppearanceFonts,
  WanderingSettings,
} from '../types/appearance'

/** Options for constructing the AppearanceConfigService. */
export interface AppearanceConfigOptions {
  /** The preset to use when none is saved or the saved one is not found. */
  defaultPreset: AppearancePreset
  /** All available presets. */
  presets: AppearancePreset[]
  /**
   * Prefix for localStorage keys. Defaults to `'nuxp'`.
   * Produces keys like `nuxp_active_preset`, `nuxp_appearance_overrides`.
   */
  storagePrefix?: string
  /**
   * Default wandering-background settings used as a fallback
   * when the preset does not define them.
   */
  defaultWanderingSettings?: WanderingSettings
  /**
   * CSS class map: preset ID -> CSS class name to apply on `<html>`.
   * If provided, the service will toggle classes when switching presets.
   */
  themeClassMap?: Record<string, string>
  /**
   * ID for the injected font style element. Defaults to `'nuxp-appearance-fonts'`.
   */
  fontStyleElementId?: string
}

/**
 * Generic appearance/theme management service.
 *
 * Persists active preset ID and user overrides to localStorage.
 * Applies colors, fonts, background, and opacity as CSS custom properties.
 */
export class AppearanceConfigService {
  private currentPreset: AppearancePreset
  private overrides: AppearanceOverrides = {}
  private applied = false

  private readonly presets: AppearancePreset[]
  private readonly defaultPreset: AppearancePreset
  private readonly storagePrefix: string
  private readonly defaultWandering: WanderingSettings | undefined
  private readonly themeClassMap: Record<string, string>
  private readonly fontStyleElementId: string

  /** Derived storage keys. */
  private readonly storageKeys: {
    ACTIVE_PRESET: string
    OVERRIDES: string
  }

  /**
   * Create a new AppearanceConfigService.
   */
  constructor(options: AppearanceConfigOptions) {
    this.defaultPreset = options.defaultPreset
    this.currentPreset = options.defaultPreset
    this.presets = options.presets
    this.storagePrefix = options.storagePrefix ?? 'nuxp'
    this.defaultWandering = options.defaultWanderingSettings
    this.themeClassMap = options.themeClassMap ?? {}
    this.fontStyleElementId = options.fontStyleElementId ?? 'nuxp-appearance-fonts'

    this.storageKeys = {
      ACTIVE_PRESET: `${this.storagePrefix}_active_preset`,
      OVERRIDES: `${this.storagePrefix}_appearance_overrides`,
    }
  }

  // ---------------------------------------------------------------------------
  // Lifecycle
  // ---------------------------------------------------------------------------

  /**
   * Initialize from localStorage and apply the current preset.
   * Call once on application startup.
   */
  initialize(): void {
    this.loadFromStorage()
    this.applyPreset()
  }

  // ---------------------------------------------------------------------------
  // Preset management
  // ---------------------------------------------------------------------------

  /**
   * Get the current active preset.
   */
  getPreset(): AppearancePreset {
    return this.currentPreset
  }

  /**
   * Get all registered presets.
   */
  getAllPresets(): AppearancePreset[] {
    return this.presets
  }

  /**
   * Find a preset by ID from the registered list.
   */
  getPresetById(id: string): AppearancePreset | undefined {
    return this.presets.find((p) => p.id === id)
  }

  /**
   * Switch to a different preset, clearing user overrides.
   */
  switchPreset(presetId: string): void {
    const preset = this.getPresetById(presetId)
    if (!preset) {
      console.error(`[AppearanceConfig] Preset "${presetId}" not found`)
      return
    }

    this.currentPreset = preset
    this.overrides = {}
    this.saveToStorage()
    this.applyPreset()
  }

  // ---------------------------------------------------------------------------
  // Effective value getters (preset + overrides)
  // ---------------------------------------------------------------------------

  /** Get effective colors (preset merged with user overrides). */
  getColors(): AppearanceColors {
    return {
      ...this.currentPreset.colors,
      ...this.overrides.colors,
    }
  }

  /** Get effective fonts (preset merged with user overrides). */
  getFonts(): AppearanceFonts {
    return {
      ...this.currentPreset.fonts,
      ...this.overrides.fonts,
    }
  }

  /** Get effective UI blur amount in pixels. */
  getUiBlur(): number {
    return this.overrides.uiBlur ?? (this.currentPreset.background.type === 'wandering' ? 20 : 0)
  }

  /** Get effective wandering background settings. */
  getWanderingSettings(): WanderingSettings | undefined {
    if (!this.defaultWandering) {
      return this.currentPreset.background.wandering
    }
    const presetWandering = this.currentPreset.background.wandering
    const overrideWandering = this.overrides.wandering
    const merged: WanderingSettings = {
      ...this.defaultWandering,
      ...presetWandering,
      ...overrideWandering,
    }
    if (!merged.images || merged.images.length === 0) {
      merged.images = this.defaultWandering.images
    }
    return merged
  }

  // ---------------------------------------------------------------------------
  // User overrides
  // ---------------------------------------------------------------------------

  /**
   * Save user overrides on top of the current preset and re-apply.
   */
  saveUserOverrides(overrides: AppearanceOverrides): void {
    this.overrides = overrides
    this.saveToStorage()
    this.applyPreset()
  }

  // ---------------------------------------------------------------------------
  // DOM application
  // ---------------------------------------------------------------------------

  /**
   * Apply the full preset (colors, fonts, background, opacity) to the DOM.
   * Safe to call only in browser environments.
   */
  applyPreset(): void {
    if (typeof document === 'undefined') return

    this.applyThemeClass()
    this.applyColors()
    this.applyFonts()
    this.applyBackground()
    this.applyOpacity()
    this.applied = true
  }

  /** Whether the appearance has been applied at least once. */
  isApplied(): boolean {
    return this.applied
  }

  /**
   * Clear all localStorage data and reset to the default preset.
   */
  resetToDefaults(): void {
    localStorage.removeItem(this.storageKeys.ACTIVE_PRESET)
    localStorage.removeItem(this.storageKeys.OVERRIDES)

    this.currentPreset = this.defaultPreset
    this.overrides = {}
    this.applyPreset()
  }

  // ---------------------------------------------------------------------------
  // Private: persistence
  // ---------------------------------------------------------------------------

  private loadFromStorage(): void {
    const savedPresetId = localStorage.getItem(this.storageKeys.ACTIVE_PRESET)
    if (savedPresetId) {
      const preset = this.getPresetById(savedPresetId)
      if (preset) {
        this.currentPreset = preset
      } else {
        console.warn(`[AppearanceConfig] Saved preset "${savedPresetId}" not found, using default`)
      }
    }

    const savedOverrides = localStorage.getItem(this.storageKeys.OVERRIDES)
    if (savedOverrides) {
      try {
        this.overrides = JSON.parse(savedOverrides)
      } catch (e) {
        console.error('[AppearanceConfig] Failed to parse overrides:', e)
        localStorage.removeItem(this.storageKeys.OVERRIDES)
      }
    }
  }

  private saveToStorage(): void {
    localStorage.setItem(this.storageKeys.ACTIVE_PRESET, this.currentPreset.id)

    if (Object.keys(this.overrides).length > 0) {
      localStorage.setItem(this.storageKeys.OVERRIDES, JSON.stringify(this.overrides))
    } else {
      localStorage.removeItem(this.storageKeys.OVERRIDES)
    }
  }

  // ---------------------------------------------------------------------------
  // Private: DOM helpers
  // ---------------------------------------------------------------------------

  private applyThemeClass(): void {
    if (Object.keys(this.themeClassMap).length === 0) return

    const root = document.documentElement
    const allClasses = Object.values(this.themeClassMap)
    allClasses.forEach((cls) => root.classList.remove(cls))

    const cssClass = this.themeClassMap[this.currentPreset.id]
    if (cssClass) {
      root.classList.add(cssClass)
    }
  }

  private applyColors(): void {
    const root = document.documentElement
    const colors = this.getColors()

    const colorMap: Record<string, string> = {
      '--text-primary': colors.textPrimary,
      '--text-secondary': colors.textSecondary,
      '--text-muted': colors.textMuted,
      '--active-tab-text': colors.activeTabText,
      '--accent-primary': colors.accentPrimary,
      '--accent-blue': colors.accentBlue,
      '--accent-green': colors.accentGreen,
      '--accent-warm': colors.accentWarm,
      '--border-dark': colors.borderDark,
    }

    for (const [prop, value] of Object.entries(colorMap)) {
      if (value) root.style.setProperty(prop, value)
    }

    if (colors.textOnAccent) {
      root.style.setProperty('--text-on-accent', colors.textOnAccent)
    }
  }

  private applyFonts(): void {
    const root = document.documentElement
    const fonts = this.getFonts()

    root.style.setProperty('--font-primary', fonts.body)
    root.style.setProperty('--font-header', fonts.header)
    root.style.setProperty('--font-menu', fonts.ui)
    root.style.setProperty('--font-mono', fonts.mono)

    document.body.style.setProperty('font-family', fonts.body, 'important')

    let style = document.getElementById(this.fontStyleElementId) as HTMLStyleElement | null
    if (!style) {
      style = document.createElement('style')
      style.id = this.fontStyleElementId
      document.head.appendChild(style)
    }

    style.textContent = `
      body, * { font-family: ${fonts.body} !important; }
      h1, h2, h3, h4, h5, h6, .font-header { font-family: ${fonts.header} !important; }
      .tab-button, .tab-label, .sub-tab-button, button, input, textarea, select, label, .font-menu { font-family: ${fonts.ui} !important; }
      code, pre, .font-mono, .color-value, .status-value { font-family: ${fonts.mono} !important; }
    `
  }

  private applyBackground(): void {
    const root = document.documentElement
    const body = document.body
    const bg = this.currentPreset.background

    root.style.removeProperty('--bg-custom-image')
    root.style.removeProperty('--bg-custom-size')
    root.style.removeProperty('--bg-image-opacity')
    root.style.removeProperty('--bg-image-blur')

    switch (bg.type) {
      case 'solid':
        root.style.setProperty('--bg-image-url', 'none')
        body.style.backgroundColor = bg.color || '#1a1a1a'
        break

      case 'image':
        root.style.removeProperty('--bg-image-url')
        body.style.removeProperty('background-color')
        break

      case 'gradient':
        root.style.setProperty('--bg-image-url', 'none')
        body.style.background = `linear-gradient(135deg, ${bg.color || '#1a1a1a'} 0%, #2d2d2d 100%)`
        break

      case 'wandering': {
        root.style.setProperty('--bg-image-url', 'none')
        body.style.backgroundColor = bg.color || '#0a0a12'
        const wandering = this.getWanderingSettings()
        if (wandering?.glassTint) {
          root.style.setProperty('--glass-tint', wandering.glassTint)
        }
        break
      }
    }
  }

  private applyOpacity(): void {
    const root = document.documentElement
    const preset = this.currentPreset
    const opacity = {
      ...preset.opacity,
      ...this.overrides.opacity,
    }

    const isLightTheme = preset.category === 'light'
    const bgColor = preset.background.color || '#1a1a1a'
    const rgb = this.hexToRgb(bgColor)

    const uiBlur = this.getUiBlur()
    root.style.setProperty('--ui-blur', `${uiBlur}px`)

    if (isLightTheme) {
      root.style.setProperty('--bg-dark', `rgba(255, 255, 255, ${opacity.panels})`)
      root.style.setProperty('--bg-darker', `rgba(245, 245, 245, ${opacity.panels})`)
      root.style.setProperty('--bg-lighter', `rgba(250, 250, 250, ${opacity.panels})`)
      root.style.setProperty('--bg-tab', `rgba(255, 255, 255, ${opacity.panels})`)
      root.style.setProperty('--bg-card', `rgba(255, 255, 255, ${opacity.cards})`)
      root.style.setProperty('--bg-hover', `rgba(240, 240, 240, ${opacity.cards})`)
      root.style.setProperty('--bg-body-transparent', `rgba(255, 255, 255, ${opacity.panels})`)
      root.style.setProperty('--bg-panel-transparent', `rgba(255, 255, 255, ${opacity.panels})`)
      root.style.setProperty('--bg-tab-content', `rgba(255, 255, 255, ${opacity.panels})`)
      root.style.setProperty('--bg-popover', `rgba(245, 245, 245, 0.92)`)
    } else {
      root.style.setProperty('--bg-dark', `rgba(${rgb.r}, ${rgb.g}, ${rgb.b}, ${opacity.panels})`)
      root.style.setProperty('--bg-darker', `rgba(${Math.max(0, rgb.r - 6)}, ${Math.max(0, rgb.g - 7)}, ${Math.max(0, rgb.b - 6)}, ${opacity.panels + 0.1})`)
      root.style.setProperty('--bg-lighter', `rgba(${rgb.r + 19}, ${rgb.g + 20}, ${rgb.b + 19}, ${opacity.panels})`)
      root.style.setProperty('--bg-tab', `rgba(${rgb.r + 5}, ${rgb.g + 12}, ${rgb.b + 5}, ${opacity.panels})`)
      root.style.setProperty('--bg-card', `rgba(${rgb.r}, ${rgb.g}, ${rgb.b}, ${opacity.cards})`)
      root.style.setProperty('--bg-hover', `rgba(${rgb.r + 19}, ${rgb.g + 30}, ${rgb.b + 19}, ${opacity.panels + 0.1})`)
      root.style.setProperty('--bg-body-transparent', `rgba(${rgb.r}, ${rgb.g}, ${rgb.b}, ${opacity.panels + 0.1})`)
      root.style.setProperty('--bg-panel-transparent', `rgba(${rgb.r}, ${rgb.g}, ${rgb.b}, ${opacity.panels})`)
      root.style.setProperty('--bg-tab-content', `rgba(${rgb.r}, ${rgb.g}, ${rgb.b}, ${opacity.panels})`)
      root.style.setProperty('--bg-popover', `rgba(${Math.max(0, rgb.r - 6)}, ${Math.max(0, rgb.g - 7)}, ${Math.max(0, rgb.b - 6)}, 0.92)`)
    }
  }

  private hexToRgb(hex: string): { r: number; g: number; b: number } {
    const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex)
    return result
      ? {
          r: parseInt(result[1], 16),
          g: parseInt(result[2], 16),
          b: parseInt(result[3], 16),
        }
      : { r: 26, g: 26, b: 26 }
  }
}
