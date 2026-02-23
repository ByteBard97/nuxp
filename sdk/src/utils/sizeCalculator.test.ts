import { describe, it, expect } from 'vitest'
import {
  inchesToPoints,
  pointsToInches,
  mmToPoints,
  pointsToMM,
  feetToInches,
  inchesToFeet,
  calculateSize,
  validateSize,
  calculateStrokeWidth,
  calculateLabelFontSize,
} from './sizeCalculator'

// ---------------------------------------------------------------------------
// Unit conversion utilities
// ---------------------------------------------------------------------------

describe('inchesToPoints', () => {
  it('converts 1 inch to 72 points', () => {
    expect(inchesToPoints(1)).toBe(72)
  })

  it('converts 0 inches to 0 points', () => {
    expect(inchesToPoints(0)).toBe(0)
  })

  it('converts fractional inches', () => {
    expect(inchesToPoints(0.5)).toBe(36)
  })
})

describe('pointsToInches', () => {
  it('converts 72 points to 1 inch', () => {
    expect(pointsToInches(72)).toBe(1)
  })

  it('converts 0 points to 0 inches', () => {
    expect(pointsToInches(0)).toBe(0)
  })

  it('round-trips with inchesToPoints', () => {
    expect(pointsToInches(inchesToPoints(3.5))).toBeCloseTo(3.5)
  })
})

describe('mmToPoints', () => {
  it('converts 25.4 mm (1 inch) to 72 points', () => {
    expect(mmToPoints(25.4)).toBeCloseTo(72)
  })

  it('converts 0 mm to 0 points', () => {
    expect(mmToPoints(0)).toBe(0)
  })
})

describe('pointsToMM', () => {
  it('converts 72 points to 25.4 mm', () => {
    expect(pointsToMM(72)).toBeCloseTo(25.4)
  })

  it('round-trips with mmToPoints', () => {
    expect(pointsToMM(mmToPoints(10))).toBeCloseTo(10)
  })
})

describe('feetToInches', () => {
  it('converts 1 foot to 12 inches', () => {
    expect(feetToInches(1)).toBe(12)
  })

  it('converts 0 feet to 0 inches', () => {
    expect(feetToInches(0)).toBe(0)
  })
})

describe('inchesToFeet', () => {
  it('converts 12 inches to 1 foot', () => {
    expect(inchesToFeet(12)).toBe(1)
  })

  it('round-trips with feetToInches', () => {
    expect(inchesToFeet(feetToInches(5))).toBe(5)
  })
})

// ---------------------------------------------------------------------------
// calculateSize
// ---------------------------------------------------------------------------

describe('calculateSize', () => {
  it('computes all fields for a known input (10ft diameter, 0.5 growth, scale 120)', () => {
    const result = calculateSize({
      diameterFt: 10,
      growthFactor: 0.5,
      drawingScale: 120,
    })

    // Input echo
    expect(result.diameterFtMature).toBe(10)
    expect(result.growthFactor).toBe(0.5)
    expect(result.drawingScale).toBe(120)

    // 10 * 0.5 = 5 feet actual diameter
    expect(result.diameterFeet).toBe(5)
    expect(result.radiusFeet).toBe(2.5)

    // 5 ft = 60 inches real, on paper = 60 / 120 = 0.5 inches
    expect(result.diameterInchesOnPaper).toBeCloseTo(0.5)
    expect(result.radiusInchesOnPaper).toBeCloseTo(0.25)

    // 0.5 inches * 72 = 36 points
    expect(result.diameterPoints).toBeCloseTo(36)
    expect(result.radiusPoints).toBeCloseTo(18)
  })

  it('returns 0 diameter on paper when drawingScale is 0', () => {
    const result = calculateSize({
      diameterFt: 10,
      growthFactor: 1,
      drawingScale: 0,
    })
    expect(result.diameterInchesOnPaper).toBe(0)
    expect(result.diameterPoints).toBe(0)
  })

  it('handles growthFactor of 1.0 (full mature size)', () => {
    const result = calculateSize({
      diameterFt: 20,
      growthFactor: 1.0,
      drawingScale: 12,
    })
    expect(result.diameterFeet).toBe(20)
    // 20 ft = 240 in / 12 = 20 inches on paper
    expect(result.diameterInchesOnPaper).toBeCloseTo(20)
  })

  it('handles growthFactor of 0 (no size)', () => {
    const result = calculateSize({
      diameterFt: 10,
      growthFactor: 0,
      drawingScale: 12,
    })
    expect(result.diameterFeet).toBe(0)
    expect(result.diameterPoints).toBe(0)
  })
})

// ---------------------------------------------------------------------------
// validateSize
// ---------------------------------------------------------------------------

describe('validateSize', () => {
  it('returns an error when diameter is zero', () => {
    const size = calculateSize({ diameterFt: 0, growthFactor: 1, drawingScale: 12 })
    const result = validateSize(size)
    expect(result.valid).toBe(false)
    expect(result.errors.length).toBeGreaterThan(0)
    expect(result.errors[0]).toMatch(/zero or negative/)
  })

  it('warns when diameter is tiny (less than 1 point)', () => {
    // Very small: 0.001 ft at scale 120 -> tiny on paper
    const size = calculateSize({ diameterFt: 0.001, growthFactor: 1, drawingScale: 120 })
    const result = validateSize(size)
    expect(result.warnings.some(w => w.includes('less than 1 point'))).toBe(true)
  })

  it('warns when diameter is huge (over 5000 points)', () => {
    // 100 ft at scale 1 -> 1200 inches on paper -> 86400 points
    const size = calculateSize({ diameterFt: 100, growthFactor: 1, drawingScale: 1 })
    const result = validateSize(size)
    expect(result.warnings.some(w => w.includes('exceeds 5000 points'))).toBe(true)
  })

  it('warns when diameter exceeds artboard width', () => {
    // 20 ft at scale 12 -> 20 inches on paper, artboard is 10 inches
    const size = calculateSize({ diameterFt: 20, growthFactor: 1, drawingScale: 12 })
    const result = validateSize(size, 10)
    expect(result.warnings.some(w => w.includes('exceeds artboard width'))).toBe(true)
  })

  it('returns valid with no warnings for a reasonable size', () => {
    const size = calculateSize({ diameterFt: 10, growthFactor: 0.5, drawingScale: 120 })
    const result = validateSize(size, 36)
    expect(result.valid).toBe(true)
    expect(result.warnings).toHaveLength(0)
    expect(result.errors).toHaveLength(0)
  })
})

// ---------------------------------------------------------------------------
// calculateStrokeWidth
// ---------------------------------------------------------------------------

describe('calculateStrokeWidth', () => {
  it('returns 1 for very small diameters (clamped minimum)', () => {
    expect(calculateStrokeWidth(0.1)).toBe(1)
  })

  it('returns 4 for very large diameters (clamped maximum)', () => {
    expect(calculateStrokeWidth(100)).toBe(4)
  })

  it('returns 1 at 0.5 inches', () => {
    expect(calculateStrokeWidth(0.5)).toBeCloseTo(1)
  })

  it('returns 4 at 4 inches', () => {
    expect(calculateStrokeWidth(4)).toBeCloseTo(4)
  })

  it('returns a mid-range value for a mid-range input', () => {
    const result = calculateStrokeWidth(2)
    expect(result).toBeGreaterThan(1)
    expect(result).toBeLessThan(4)
  })
})

// ---------------------------------------------------------------------------
// calculateLabelFontSize
// ---------------------------------------------------------------------------

describe('calculateLabelFontSize', () => {
  it('returns 6 for very small diameters (clamped minimum)', () => {
    expect(calculateLabelFontSize(0.01)).toBe(6)
  })

  it('returns 72 for very large diameters (clamped maximum)', () => {
    expect(calculateLabelFontSize(100)).toBe(72)
  })

  it('scales at roughly 40% of diameter in points', () => {
    // 1 inch -> 72 * 0.4 = 28.8
    expect(calculateLabelFontSize(1)).toBeCloseTo(28.8)
  })

  it('returns a value within the clamped range for moderate inputs', () => {
    const result = calculateLabelFontSize(0.5)
    expect(result).toBeGreaterThanOrEqual(6)
    expect(result).toBeLessThanOrEqual(72)
  })
})
