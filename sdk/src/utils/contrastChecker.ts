/**
 * WCAG Contrast Ratio Checker
 *
 * Pure utility functions for checking text/background color contrast
 * against WCAG 2.1 accessibility guidelines.
 *
 * AA minimum: 4.5:1 for normal text, 3:1 for large text (>=18px bold or >=24px).
 */

/**
 * Parse any CSS color string to { r, g, b } in 0-255 range.
 * Supports hex (#RRGGBB), rgb(), and rgba() formats.
 * Falls back to a DOM element for computed colors (hsl, named colors, etc.).
 *
 * @param color - CSS color string
 * @returns RGB components or null if unparseable
 */
function parseColor(color: string): { r: number; g: number; b: number } | null {
  // Try hex
  const hex = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(color)
  if (hex) {
    return { r: parseInt(hex[1], 16), g: parseInt(hex[2], 16), b: parseInt(hex[3], 16) }
  }

  // Try rgb/rgba
  const rgb = /rgba?\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)/.exec(color)
  if (rgb) {
    return { r: parseInt(rgb[1]), g: parseInt(rgb[2]), b: parseInt(rgb[3]) }
  }

  // Use a temp DOM element for computed colors (hsl, named colors, etc.)
  try {
    const el = document.createElement('div')
    el.style.color = color
    el.style.display = 'none'
    document.body.appendChild(el)
    const computed = getComputedStyle(el).color
    document.body.removeChild(el)
    const match = /rgba?\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)/.exec(computed)
    if (match) {
      return { r: parseInt(match[1]), g: parseInt(match[2]), b: parseInt(match[3]) }
    }
  } catch {
    // Ignore -- DOM may not be available
  }

  return null
}

/**
 * Calculate WCAG relative luminance for an sRGB color.
 *
 * @param r - Red channel (0-255)
 * @param g - Green channel (0-255)
 * @param b - Blue channel (0-255)
 * @returns Relative luminance in range 0-1
 */
function relativeLuminance(r: number, g: number, b: number): number {
  const [rs, gs, bs] = [r, g, b].map(c => {
    const s = c / 255
    return s <= 0.03928 ? s / 12.92 : Math.pow((s + 0.055) / 1.055, 2.4)
  })
  return 0.2126 * rs + 0.7152 * gs + 0.0722 * bs
}

/**
 * Calculate the WCAG contrast ratio between two colors.
 *
 * @param fg - Foreground CSS color string
 * @param bg - Background CSS color string
 * @returns Contrast ratio from 1:1 (identical) to 21:1 (black on white).
 *          Returns 21 if either color cannot be parsed (assumes OK).
 */
export function contrastRatio(fg: string, bg: string): number {
  const fgRgb = parseColor(fg)
  const bgRgb = parseColor(bg)
  if (!fgRgb || !bgRgb) return 21 // Can't parse -- assume OK

  const l1 = relativeLuminance(fgRgb.r, fgRgb.g, fgRgb.b)
  const l2 = relativeLuminance(bgRgb.r, bgRgb.g, bgRgb.b)

  const lighter = Math.max(l1, l2)
  const darker = Math.min(l1, l2)
  return (lighter + 0.05) / (darker + 0.05)
}

/**
 * Check if the contrast ratio between two colors meets WCAG AA for normal text (4.5:1).
 *
 * @param fg - Foreground CSS color string
 * @param bg - Background CSS color string
 */
export function meetsAA(fg: string, bg: string): boolean {
  return contrastRatio(fg, bg) >= 4.5
}

/**
 * Check if the contrast ratio between two colors meets WCAG AA for large text (3:1).
 * Large text is defined as >= 18px bold or >= 24px regular.
 *
 * @param fg - Foreground CSS color string
 * @param bg - Background CSS color string
 */
export function meetsAALarge(fg: string, bg: string): boolean {
  return contrastRatio(fg, bg) >= 3.0
}

/**
 * A detected contrast issue on a specific element.
 */
export interface ContrastIssue {
  /** CSS selector or tag description of the element */
  element: string
  /** Foreground (text) color */
  foreground: string
  /** Effective background color */
  background: string
  /** Measured contrast ratio */
  ratio: number
  /** Required contrast ratio for the element's text size */
  required: number
}

/**
 * Walk up the DOM tree to find the first ancestor with an opaque background.
 *
 * @param el - Starting element
 * @returns Background color string, or null if none found
 */
function getEffectiveBackground(el: HTMLElement): string | null {
  let current: HTMLElement | null = el
  while (current) {
    const style = getComputedStyle(current)
    const bg = style.backgroundColor
    if (bg && bg !== 'rgba(0, 0, 0, 0)' && bg !== 'transparent') {
      return bg
    }
    current = current.parentElement
  }
  return null
}

/**
 * Scan the document for text elements with low contrast.
 * Walks all visible text-bearing elements and checks them against WCAG AA.
 *
 * @returns Array of detected contrast issues
 */
export function scanForContrastIssues(): ContrastIssue[] {
  const issues: ContrastIssue[] = []
  const seen = new Set<Element>()

  const allElements = document.querySelectorAll(
    'h1, h2, h3, h4, h5, h6, p, span, a, button, label, input, textarea, select, ' +
    'div, li, td, th, dt, dd, figcaption, summary, legend, option'
  )

  allElements.forEach((el) => {
    if (seen.has(el)) return
    const htmlEl = el as HTMLElement

    const style = getComputedStyle(htmlEl)
    if (style.display === 'none' || style.visibility === 'hidden' || style.opacity === '0') return

    const hasDirectText = Array.from(htmlEl.childNodes).some(
      node => node.nodeType === Node.TEXT_NODE && node.textContent?.trim()
    )
    if (!hasDirectText) return

    const fg = style.color
    const bg = getEffectiveBackground(htmlEl)
    if (!bg) return

    const ratio = contrastRatio(fg, bg)
    const fontSize = parseFloat(style.fontSize)
    const isBold = parseInt(style.fontWeight) >= 700
    const isLargeText = fontSize >= 24 || (fontSize >= 18.66 && isBold)
    const required = isLargeText ? 3.0 : 4.5

    if (ratio < required) {
      seen.add(el)
      const descriptor = htmlEl.className
        ? `${htmlEl.tagName.toLowerCase()}.${htmlEl.className.split(' ')[0]}`
        : htmlEl.tagName.toLowerCase()
      issues.push({
        element: descriptor,
        foreground: fg,
        background: bg,
        ratio: Math.round(ratio * 100) / 100,
        required,
      })
    }
  })

  return issues
}

/**
 * Add visual warning indicators (red dashed outlines) to low-contrast elements.
 * Removes any previous highlights before scanning.
 *
 * @returns Number of elements highlighted
 */
export function highlightContrastIssues(): number {
  // Remove previous highlights
  document.querySelectorAll('[data-contrast-warning]').forEach(el => {
    (el as HTMLElement).removeAttribute('data-contrast-warning');
    (el as HTMLElement).style.removeProperty('outline');
    (el as HTMLElement).style.removeProperty('outline-offset')
  })

  const seen = new Set<Element>()

  const allElements = document.querySelectorAll(
    'h1, h2, h3, h4, h5, h6, p, span, a, button, label, input, textarea, select, ' +
    'div, li, td, th, dt, dd, figcaption, summary, legend, option'
  )

  allElements.forEach((el) => {
    if (seen.has(el)) return
    const htmlEl = el as HTMLElement

    const style = getComputedStyle(htmlEl)
    if (style.display === 'none' || style.visibility === 'hidden' || style.opacity === '0') return

    const hasDirectText = Array.from(htmlEl.childNodes).some(
      node => node.nodeType === Node.TEXT_NODE && node.textContent?.trim()
    )
    if (!hasDirectText) return

    const fg = style.color
    const bg = getEffectiveBackground(htmlEl)
    if (!bg) return

    const ratio = contrastRatio(fg, bg)
    const fontSize = parseFloat(style.fontSize)
    const isBold = parseInt(style.fontWeight) >= 700
    const isLargeText = fontSize >= 24 || (fontSize >= 18.66 && isBold)
    const required = isLargeText ? 3.0 : 4.5

    if (ratio < required) {
      seen.add(el)
      htmlEl.setAttribute('data-contrast-warning', `${ratio.toFixed(1)}:1`)
      htmlEl.style.outline = '2px dashed #ff6b6b'
      htmlEl.style.outlineOffset = '2px'
    }
  })

  return seen.size
}
