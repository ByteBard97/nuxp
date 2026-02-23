/**
 * Font Configuration Service
 *
 * Manages font assignments and applies them as CSS custom properties.
 * Consuming applications provide their own font configuration object
 * at construction time -- no built-in font presets.
 *
 * @example
 * ```ts
 * const fontService = new FontConfigService({
 *   fonts: {
 *     inter: 'Inter, sans-serif',
 *     jetbrains: 'JetBrains Mono, monospace',
 *   },
 *   assignments: {
 *     header: 'inter',
 *     menu: 'inter',
 *     body: 'inter',
 *     mono: 'jetbrains',
 *   },
 *   baseFontSize: 13,
 * })
 *
 * fontService.applyFonts()
 * ```
 */

/**
 * Font configuration shape.
 * Maps logical font keys to font-family strings, and assigns
 * them to semantic roles (header, menu, body, mono).
 */
export interface FontConfig {
  /** Map of font key to CSS font-family value */
  fonts: Record<string, string>
  /** Semantic font role assignments (values are keys into `fonts`) */
  assignments: {
    header: string
    menu: string
    body: string
    mono: string
  }
  /** Base font size in pixels */
  baseFontSize: number
}

/**
 * Service for managing font configuration and applying it to the DOM.
 */
export class FontConfigService {
  private config: FontConfig
  private applied = false
  private styleElementId: string

  /**
   * Create a new FontConfigService.
   *
   * @param config - The font configuration to use.
   * @param styleElementId - The id attribute for the injected style element.
   *   Defaults to `'nuxp-font-config'`.
   */
  constructor(config: FontConfig, styleElementId = 'nuxp-font-config') {
    this.config = config
    this.styleElementId = styleElementId
  }

  /**
   * Get the CSS font-family string for a font key.
   *
   * @param fontKey - A key from the `fonts` map.
   * @returns The font-family string.
   * @throws If the key is not found in the font map.
   */
  getFont(fontKey: string): string {
    const font = this.config.fonts[fontKey]
    if (!font) {
      throw new Error(`[FontConfigService] Unknown font key: "${fontKey}"`)
    }
    return font
  }

  /**
   * Get the font-family string for a semantic assignment.
   *
   * @param assignment - One of 'header', 'menu', 'body', 'mono'.
   * @returns The resolved font-family string.
   */
  getAssignedFont(assignment: keyof FontConfig['assignments']): string {
    const fontKey = this.config.assignments[assignment]
    return this.getFont(fontKey)
  }

  /**
   * Get the configured base font size in pixels.
   */
  getBaseFontSize(): number {
    return this.config.baseFontSize
  }

  /**
   * Get the full font configuration object.
   */
  getConfig(): FontConfig {
    return this.config
  }

  /**
   * Replace the current configuration at runtime.
   *
   * @param config - New font configuration.
   */
  setConfig(config: FontConfig): void {
    this.config = config
    this.applied = false
  }

  /**
   * Apply font settings to the document by setting CSS custom properties
   * and injecting a style element.
   *
   * Safe to call in browser environments only (requires `document`).
   */
  applyFonts(): void {
    if (typeof document === 'undefined') return

    const root = document.documentElement
    const headerFont = this.getAssignedFont('header')
    const menuFont = this.getAssignedFont('menu')
    const bodyFont = this.getAssignedFont('body')
    const monoFont = this.getAssignedFont('mono')
    const fontSize = this.getBaseFontSize()

    // Set CSS custom properties
    root.style.setProperty('--font-primary', bodyFont)
    root.style.setProperty('--font-header', headerFont)
    root.style.setProperty('--font-menu', menuFont)
    root.style.setProperty('--font-mono', monoFont)
    root.style.setProperty('--font-size-base', `${fontSize}px`)

    // Apply to body
    document.body.style.setProperty('font-family', bodyFont, 'important')
    document.body.style.setProperty('font-size', `${fontSize}px`, 'important')

    // Inject style element for broad CSS coverage
    let style = document.getElementById(this.styleElementId) as HTMLStyleElement | null
    if (!style) {
      style = document.createElement('style')
      style.id = this.styleElementId
      document.head.appendChild(style)
    }

    style.textContent = `
      body, * { font-family: ${bodyFont} !important; }
      h1, h2, h3, h4, h5, h6, .font-header { font-family: ${headerFont} !important; }
      .tab-button, .tab-label, .sub-tab-button, button, input, textarea, select, label, .font-menu { font-family: ${menuFont} !important; }
      code, pre, .font-mono, .color-value, .status-value { font-family: ${monoFont} !important; }
    `

    this.applied = true
  }

  /**
   * Whether fonts have been applied to the DOM.
   */
  isApplied(): boolean {
    return this.applied
  }

  /**
   * List all available font keys and their font-family values.
   */
  getAvailableFonts(): Record<string, string> {
    return { ...this.config.fonts }
  }
}
