/**
 * Size Calculator -- Pure math utilities for scaled drawing
 *
 * Converts real-world dimensions to on-paper sizes given a drawing scale,
 * with support for growth/time factors. All calculations are self-contained
 * with no external dependencies.
 *
 * Unit conventions:
 * - Real-world measurements: feet
 * - Paper measurements: inches
 * - Illustrator native unit: points (1 inch = 72 points)
 */

/** Points per inch (Illustrator / PostScript standard) */
const POINTS_PER_INCH = 72

/** Inches per foot */
const INCHES_PER_FOOT = 12

/** Millimeters per inch */
const MM_PER_INCH = 25.4

// ---------------------------------------------------------------------------
// Input / Output interfaces
// ---------------------------------------------------------------------------

/**
 * Input parameters for a size calculation.
 */
export interface SizeCalculationInput {
  /** Diameter in feet at full maturity */
  diameterFt: number
  /** Growth factor (0.0 - 1.0, where 1.0 = full mature size) */
  growthFactor: number
  /** Drawing scale ratio -- e.g. 12 means 1" on paper = 12" (1 ft) in reality */
  drawingScale: number
}

/**
 * Complete size calculation result with all unit representations.
 */
export interface SizeCalculation {
  /** Input: diameter in feet at full maturity */
  diameterFtMature: number
  /** Input: growth factor applied */
  growthFactor: number
  /** Input: drawing scale */
  drawingScale: number
  /** Actual diameter in feet (with growth factor applied) */
  diameterFeet: number
  /** Actual radius in feet */
  radiusFeet: number
  /** Diameter on paper in inches */
  diameterInchesOnPaper: number
  /** Radius on paper in inches */
  radiusInchesOnPaper: number
  /** Diameter in points (Illustrator unit) */
  diameterPoints: number
  /** Radius in points */
  radiusPoints: number
}

/**
 * Validation result for a size calculation.
 */
export interface ValidationResult {
  /** Whether the size is valid for placement */
  valid: boolean
  /** Non-fatal warnings (e.g. unusually large) */
  warnings: string[]
  /** Fatal errors (e.g. zero or negative size) */
  errors: string[]
}

// ---------------------------------------------------------------------------
// Unit conversion utilities
// ---------------------------------------------------------------------------

/**
 * Convert inches to points.
 * @param inches - Value in inches
 * @returns Value in points
 */
export function inchesToPoints(inches: number): number {
  return inches * POINTS_PER_INCH
}

/**
 * Convert points to inches.
 * @param points - Value in points
 * @returns Value in inches
 */
export function pointsToInches(points: number): number {
  return points / POINTS_PER_INCH
}

/**
 * Convert millimeters to points.
 * @param mm - Value in millimeters
 * @returns Value in points
 */
export function mmToPoints(mm: number): number {
  return (mm / MM_PER_INCH) * POINTS_PER_INCH
}

/**
 * Convert points to millimeters.
 * @param points - Value in points
 * @returns Value in millimeters
 */
export function pointsToMM(points: number): number {
  return (points / POINTS_PER_INCH) * MM_PER_INCH
}

/**
 * Convert feet to inches.
 * @param feet - Value in feet
 * @returns Value in inches
 */
export function feetToInches(feet: number): number {
  return feet * INCHES_PER_FOOT
}

/**
 * Convert inches to feet.
 * @param inches - Value in inches
 * @returns Value in feet
 */
export function inchesToFeet(inches: number): number {
  return inches / INCHES_PER_FOOT
}

// ---------------------------------------------------------------------------
// Size calculation
// ---------------------------------------------------------------------------

/**
 * Calculate all size representations from input parameters.
 *
 * @param input - Diameter, growth factor, and drawing scale
 * @returns Complete size calculation with every unit representation
 */
export function calculateSize(input: SizeCalculationInput): SizeCalculation {
  const { diameterFt, growthFactor, drawingScale } = input

  // Apply growth factor
  const diameterFeet = diameterFt * growthFactor
  const radiusFeet = diameterFeet / 2

  // Convert to real-world inches, then scale to paper inches
  const diameterRealInches = feetToInches(diameterFeet)
  const diameterInchesOnPaper = drawingScale > 0 ? diameterRealInches / drawingScale : 0
  const radiusInchesOnPaper = diameterInchesOnPaper / 2

  // Convert paper inches to points
  const diameterPoints = inchesToPoints(diameterInchesOnPaper)
  const radiusPoints = diameterPoints / 2

  return {
    diameterFtMature: diameterFt,
    growthFactor,
    drawingScale,
    diameterFeet,
    radiusFeet,
    diameterInchesOnPaper,
    radiusInchesOnPaper,
    diameterPoints,
    radiusPoints,
  }
}

/**
 * Validate a size calculation for reasonableness.
 *
 * @param size - A previously computed SizeCalculation
 * @param artboardWidthInches - Optional artboard width for scale sanity check
 * @returns Validation result with warnings and errors
 */
export function validateSize(size: SizeCalculation, artboardWidthInches?: number): ValidationResult {
  const warnings: string[] = []
  const errors: string[] = []

  if (size.diameterPoints <= 0) {
    errors.push('Diameter is zero or negative -- check inputs.')
  }

  if (size.diameterPoints < 1) {
    warnings.push('Diameter on paper is less than 1 point -- item may be invisible.')
  }

  if (size.diameterPoints > 5000) {
    warnings.push('Diameter on paper exceeds 5000 points -- check drawing scale.')
  }

  if (artboardWidthInches && size.diameterInchesOnPaper > artboardWidthInches) {
    warnings.push('Item diameter exceeds artboard width -- verify scale settings.')
  }

  return {
    valid: errors.length === 0,
    warnings,
    errors,
  }
}

/**
 * Calculate a reasonable stroke width for a circle outline.
 *
 * @param diameterInches - Diameter in inches on paper
 * @returns Stroke width in points (clamped to 1-4 pt)
 */
export function calculateStrokeWidth(diameterInches: number): number {
  // Scale linearly: 0.5" -> 1pt, 4"+ -> 4pt
  const raw = 1 + (diameterInches - 0.5) * (3 / 3.5)
  return Math.max(1, Math.min(4, raw))
}

/**
 * Calculate a reasonable font size for a label placed on a circle.
 *
 * @param diameterInches - Diameter in inches on paper
 * @returns Font size in points (clamped to 6-72 pt)
 */
export function calculateLabelFontSize(diameterInches: number): number {
  // Rough heuristic: font ≈ 40% of diameter, clamped
  const raw = diameterInches * POINTS_PER_INCH * 0.4
  return Math.max(6, Math.min(72, raw))
}
