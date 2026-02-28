/**
 * Core Geometry Types
 *
 * Generic geometry primitives used throughout the SDK.
 */

/**
 * A 2D point in coordinate space.
 */
export interface Point {
  x: number
  y: number
}

/**
 * A polygon defined by an outer boundary and optional holes.
 */
export interface Polygon {
  /** Outer boundary points (wound counter-clockwise by convention) */
  points: Point[]
  /** Optional inner hole boundaries (wound clockwise by convention) */
  holes?: Point[][]
}

/**
 * A tint/color overlay style applied to artwork.
 */
export interface TintStyle {
  /** Opacity (0-1) */
  alpha: number
  /** Blend mode name */
  mode: string
  /** Optional hex color */
  color?: string
}

/**
 * Supported fill pattern types for area fills.
 */
export type FillPattern = 'square' | 'triangular' | 'hexagonal'

/**
 * Parameters for filling a polygon region with evenly spaced items.
 */
export interface FillParams {
  /** The polygon region to fill */
  polygon: Polygon
  /** Spacing between items (in document units) */
  spacing: number
  /** Grid pattern type */
  pattern: FillPattern
  /** Pattern rotation in degrees */
  rotation: number
  /** Inset distance from the polygon boundary */
  inset: number
  /** Random positional jitter amount */
  jitter: number
  /** Identifier for the item type being placed */
  itemId: string
  /** Drawing scale factor */
  scale: number
}

/**
 * Result of a document healing/reconciliation operation.
 */
export interface HealResult {
  /** Number of items added */
  added: number
  /** Number of items updated */
  updated: number
  /** Number of orphaned items found */
  orphans: number
  /** Number of items moved */
  moved: number
}
