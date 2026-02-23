import { describe, it, expect } from 'vitest'
import { contrastRatio, meetsAA, meetsAALarge } from './contrastChecker'

describe('contrastRatio', () => {
  it('returns 21 for black on white', () => {
    const ratio = contrastRatio('#000000', '#ffffff')
    expect(ratio).toBeCloseTo(21, 0)
  })

  it('returns 21 for white on black (order does not matter)', () => {
    const ratio = contrastRatio('#ffffff', '#000000')
    expect(ratio).toBeCloseTo(21, 0)
  })

  it('returns 1 for identical colors', () => {
    expect(contrastRatio('#336699', '#336699')).toBeCloseTo(1)
  })

  it('returns a known mid-range value for gray on white', () => {
    // #767676 on white is right around 4.54:1 (the minimum AA-passing gray)
    const ratio = contrastRatio('#767676', '#ffffff')
    expect(ratio).toBeGreaterThan(4.5)
    expect(ratio).toBeLessThan(5.0)
  })

  it('handles rgb() format', () => {
    const ratio = contrastRatio('rgb(0, 0, 0)', 'rgb(255, 255, 255)')
    expect(ratio).toBeCloseTo(21, 0)
  })

  it('handles rgba() format (alpha ignored for contrast)', () => {
    const ratio = contrastRatio('rgba(0, 0, 0, 1)', 'rgba(255, 255, 255, 1)')
    expect(ratio).toBeCloseTo(21, 0)
  })

  it('returns 21 when a color cannot be parsed (assumes OK)', () => {
    // In a node environment without DOM, named colors cannot be parsed
    const ratio = contrastRatio('not-a-color', '#ffffff')
    expect(ratio).toBe(21)
  })
})

describe('meetsAA', () => {
  it('passes for black on white (ratio ~21)', () => {
    expect(meetsAA('#000000', '#ffffff')).toBe(true)
  })

  it('passes at exactly 4.5 threshold', () => {
    // #767676 on white is ~4.54
    expect(meetsAA('#767676', '#ffffff')).toBe(true)
  })

  it('fails for low-contrast colors', () => {
    // #aaaaaa on white is roughly 2.32:1
    expect(meetsAA('#aaaaaa', '#ffffff')).toBe(false)
  })

  it('fails for identical colors (ratio 1)', () => {
    expect(meetsAA('#ffffff', '#ffffff')).toBe(false)
  })
})

describe('meetsAALarge', () => {
  it('passes for black on white', () => {
    expect(meetsAALarge('#000000', '#ffffff')).toBe(true)
  })

  it('passes at the 3.0 threshold', () => {
    // #949494 on white is roughly 3.03:1
    expect(meetsAALarge('#949494', '#ffffff')).toBe(true)
  })

  it('fails for very low contrast', () => {
    // #cccccc on white is roughly 1.6:1
    expect(meetsAALarge('#cccccc', '#ffffff')).toBe(false)
  })

  it('fails for identical colors (ratio 1)', () => {
    expect(meetsAALarge('#ffffff', '#ffffff')).toBe(false)
  })
})

// scanForContrastIssues and highlightContrastIssues require a full DOM
// environment (document.querySelectorAll, getComputedStyle, etc.)
// and are not testable in a Node environment.
describe.todo('scanForContrastIssues (requires DOM)')
describe.todo('highlightContrastIssues (requires DOM)')
