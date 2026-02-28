/**
 * CoordinateSystemManager - Unified coordinate transformation utilities
 *
 * Handles coordinate conversions and transformations between:
 * - D3.js coordinate space (origin at top-left, Y increases downward)
 * - Illustrator coordinate space (origin at bottom-left, Y increases upward)
 * - Artboard-relative coordinates
 *
 * Uses d3-polygon for robust geometry operations instead of custom implementations.
 */

import { polygonArea, polygonCentroid } from 'd3-polygon'
import type { Point } from '../types/core'

export type { Point }

export interface Bounds {
  minX: number
  maxX: number
  minY: number
  maxY: number
}

export interface Dimensions {
  width: number
  height: number
}

export interface EllipseParams {
  left: number
  top: number
  width: number
  height: number
}

/**
 * CoordinateSystemManager - Static utility class for coordinate operations
 */
export class CoordinateSystemManager {
  /**
   * Calculate bounding box from array of points
   */
  static getBounds(points: Point[]): Bounds {
    if (points.length === 0) {
      throw new Error('Cannot calculate bounds for empty point array')
    }

    return {
      minX: Math.min(...points.map(p => p.x)),
      maxX: Math.max(...points.map(p => p.x)),
      minY: Math.min(...points.map(p => p.y)),
      maxY: Math.max(...points.map(p => p.y))
    }
  }

  /**
   * Get width and height from bounds
   */
  static getBoundsDimensions(bounds: Bounds): Dimensions {
    return {
      width: bounds.maxX - bounds.minX,
      height: bounds.maxY - bounds.minY
    }
  }

  /**
   * Get center point of bounds
   */
  static getBoundsCenter(bounds: Bounds): Point {
    return {
      x: (bounds.minX + bounds.maxX) / 2,
      y: (bounds.minY + bounds.maxY) / 2
    }
  }

  /**
   * Translate an array of points by an offset
   */
  static translatePoints(points: Point[], offset: Point): Point[] {
    return points.map(p => ({
      x: p.x + offset.x,
      y: p.y + offset.y
    }))
  }

  /**
   * Translate a single point by an offset
   */
  static translatePoint(point: Point, offset: Point): Point {
    return {
      x: point.x + offset.x,
      y: point.y + offset.y
    }
  }

  /**
   * Scale points from one coordinate space to another
   */
  static scalePoints(points: Point[], scale: number): Point[] {
    return points.map(p => ({
      x: p.x * scale,
      y: p.y * scale
    }))
  }

  /**
   * Calculate offset needed to center source bounds within target bounds
   */
  static calculateCenteringOffset(sourceBounds: Bounds, targetBounds: Bounds): Point {
    const sourceCenter = this.getBoundsCenter(sourceBounds)
    const targetCenter = this.getBoundsCenter(targetBounds)

    return {
      x: targetCenter.x - sourceCenter.x,
      y: targetCenter.y - sourceCenter.y
    }
  }

  /**
   * Calculate Euclidean distance between two points
   */
  static distance(p1: Point, p2: Point): number {
    const dx = p2.x - p1.x
    const dy = p2.y - p1.y
    return Math.sqrt(dx * dx + dy * dy)
  }

  /**
   * Calculate midpoint between two points
   */
  static midpoint(p1: Point, p2: Point): Point {
    return {
      x: (p1.x + p2.x) / 2,
      y: (p1.y + p2.y) / 2
    }
  }

  /**
   * Check if two circles overlap
   */
  static circlesOverlap(center1: Point, radius1: number, center2: Point, radius2: number): boolean {
    const dist = this.distance(center1, center2)
    return dist < radius1 + radius2
  }

  /**
   * Convert D3 coordinates to Illustrator coordinates
   * D3: origin top-left, Y increases downward
   * Illustrator: origin bottom-left, Y increases upward
   *
   * NOTE: These conversion methods exist but are currently UNUSED in the codebase.
   * This suggests the coordinate systems may actually be compatible in practice,
   * OR we're missing necessary conversions. Needs investigation.
   */
  static d3ToIllustrator(point: Point, artboardHeight: number): Point {
    return {
      x: point.x,
      y: artboardHeight - point.y
    }
  }

  /**
   * Convert Illustrator coordinates to D3 coordinates
   */
  static illustratorToD3(point: Point, artboardHeight: number): Point {
    return {
      x: point.x,
      y: artboardHeight - point.y
    }
  }

  /**
   * Validate that a circle fits within bounds
   * Useful for catching coordinate system bugs early
   *
   * @param center - Circle center point
   * @param radius - Circle radius
   * @param bounds - Bounds to check against
   * @param label - Optional label for error messages
   * @throws Error if circle extends outside bounds
   */
  static validateCircleInBounds(
    center: Point,
    radius: number,
    bounds: Bounds,
    label: string = 'Circle'
  ): void {
    const errors: string[] = []

    if (center.x - radius < bounds.minX) {
      errors.push(`${label} extends beyond LEFT edge: ${center.x - radius} < ${bounds.minX}`)
    }
    if (center.x + radius > bounds.maxX) {
      errors.push(`${label} extends beyond RIGHT edge: ${center.x + radius} > ${bounds.maxX}`)
    }
    if (center.y - radius < bounds.minY) {
      errors.push(`${label} extends beyond TOP edge: ${center.y - radius} < ${bounds.minY}`)
    }
    if (center.y + radius > bounds.maxY) {
      errors.push(`${label} extends beyond BOTTOM edge: ${center.y + radius} > ${bounds.maxY}`)
    }

    if (errors.length > 0) {
      throw new Error(
        `Validation failed for ${label} at (${center.x.toFixed(2)}, ${center.y.toFixed(2)}) with radius ${radius.toFixed(2)}:\n${
        errors.join('\n')}`
      )
    }
  }

  /**
   * Validate bounds (ensure min < max)
   * @throws Error if bounds are invalid
   */
  static validateBounds(bounds: Bounds): void {
    if (bounds.minX > bounds.maxX) {
      throw new Error(`Invalid bounds: minX (${bounds.minX}) > maxX (${bounds.maxX})`)
    }
    if (bounds.minY > bounds.maxY) {
      throw new Error(`Invalid bounds: minY (${bounds.minY}) > maxY (${bounds.maxY})`)
    }
  }

  /**
   * Inset bounds (shrink inward by a margin)
   * @throws Error if inset creates invalid bounds
   */
  static insetBounds(bounds: Bounds, inset: number): Bounds {
    const result = {
      minX: bounds.minX + inset,
      maxX: bounds.maxX - inset,
      minY: bounds.minY + inset,
      maxY: bounds.maxY - inset
    }
    this.validateBounds(result)
    return result
  }

  /**
   * Get bounding box for a circle
   */
  static getCircleBounds(center: Point, radius: number): Bounds {
    return {
      minX: center.x - radius,
      maxX: center.x + radius,
      minY: center.y - radius,
      maxY: center.y + radius
    }
  }

  /**
   * Rotate an array of points around a center point
   * @param points - Points to rotate
   * @param angleDegrees - Rotation angle in degrees (positive = counterclockwise)
   * @param center - Center point of rotation
   * @returns Rotated points
   */
  static rotatePoints(points: Point[], angleDegrees: number, center: Point): Point[] {
    const angleRadians = (angleDegrees * Math.PI) / 180
    const cos = Math.cos(angleRadians)
    const sin = Math.sin(angleRadians)

    return points.map(point => {
      const dx = point.x - center.x
      const dy = point.y - center.y

      return {
        x: center.x + dx * cos - dy * sin,
        y: center.y + dx * sin + dy * cos
      }
    })
  }

  /**
   * Calculate polygon area using d3-polygon library
   * @param points - Polygon vertices in order
   * @returns Area of polygon
   */
  static calculatePolygonArea(points: Point[]): number {
    if (points.length < 3) {
      return 0
    }

    // Convert Point[] to [number, number][] format for d3-polygon
    const polygon = points.map(p => [p.x, p.y] as [number, number])
    return Math.abs(polygonArea(polygon))
  }

  /**
   * Calculate polygon centroid using d3-polygon library
   * @param points - Polygon vertices in order
   * @returns Centroid point
   */
  static calculatePolygonCentroid(points: Point[]): Point {
    if (points.length < 3) {
      return { x: 0, y: 0 }
    }

    // Convert Point[] to [number, number][] format for d3-polygon
    const polygon = points.map(p => [p.x, p.y] as [number, number])
    const [x, y] = polygonCentroid(polygon)
    return { x, y }
  }

  /**
   * Rotate a single point around a center point
   * @param point - Point to rotate
   * @param angleDegrees - Rotation angle in degrees (positive = counterclockwise)
   * @param center - Center point of rotation
   * @returns Rotated point
   */
  static rotatePoint(point: Point, angleDegrees: number, center: Point): Point {
    return this.rotatePoints([point], angleDegrees, center)[0]
  }

  /**
   * Scale a single point from a center
   * @param point - Point to scale
   * @param scale - Scale factor
   * @param center - Center point for scaling (default: origin)
   * @returns Scaled point
   */
  static scalePoint(point: Point, scale: number, center: Point = { x: 0, y: 0 }): Point {
    const dx = point.x - center.x
    const dy = point.y - center.y
    return {
      x: center.x + dx * scale,
      y: center.y + dy * scale
    }
  }

  // ============================================================================
  // Unit Conversion Methods
  // Note: These delegate to the actual conversion logic in sizeCalculator
  // They're kept here for backward compatibility with existing code
  // ============================================================================

  /**
   * Convert inches to points (Illustrator's native unit)
   */
  static inchesToPoints(inches: number): number {
    return inches * 72 // 72 points per inch
  }

  /**
   * Convert points to inches
   */
  static pointsToInches(points: number): number {
    return points / 72
  }

  /**
   * Convert feet to inches
   */
  static feetToInches(feet: number): number {
    return feet * 12
  }

  /**
   * Convert inches to feet
   */
  static inchesToFeet(inches: number): number {
    return inches / 12
  }

  /**
   * Convert drawing scale to feet per inch ratio
   * @param scale - Drawing scale (e.g., 12 means 1" on paper = 12" in reality)
   * @returns Feet per inch on paper
   */
  static scaleToFeetPerInch(scale: number): number {
    return scale / 12
  }

  /**
   * Check if a point is within bounds
   */
  static isPointInBounds(point: Point, bounds: Bounds): boolean {
    return (
      point.x >= bounds.minX &&
      point.x <= bounds.maxX &&
      point.y >= bounds.minY &&
      point.y <= bounds.maxY
    )
  }

  /**
   * Check if two bounds intersect
   */
  static boundsIntersect(bounds1: Bounds, bounds2: Bounds): boolean {
    return !(
      bounds1.maxX < bounds2.minX ||
      bounds1.minX > bounds2.maxX ||
      bounds1.maxY < bounds2.minY ||
      bounds1.minY > bounds2.maxY
    )
  }

  /**
   * Expand bounds outward by a margin
   */
  static expandBounds(bounds: Bounds, margin: number): Bounds {
    return {
      minX: bounds.minX - margin,
      maxX: bounds.maxX + margin,
      minY: bounds.minY - margin,
      maxY: bounds.maxY + margin
    }
  }

  /**
   * Apply drawing scale (convert real-world to paper coordinates)
   * @param value - Number or Point in real-world coordinates (inches)
   * @param scale - Drawing scale factor (e.g., 120 means 1" on paper = 120" = 10' in reality)
   * @returns Number or Point scaled for paper
   */
  static applyDrawingScale(value: number, scale: number): number
  static applyDrawingScale(value: Point, scale: number): Point
  static applyDrawingScale(value: number | Point, scale: number): number | Point {
    if (scale <= 0) {
      throw new Error('Invalid drawing scale: must be positive')
    }

    if (typeof value === 'number') {
      return value / scale
    } else {
      return {
        x: value.x / scale,
        y: value.y / scale
      }
    }
  }

  /**
   * Remove drawing scale (convert paper coordinates to real-world)
   * @param value - Number or Point in paper coordinates (inches)
   * @param scale - Drawing scale factor
   * @returns Number or Point in real-world coordinates
   */
  static removeDrawingScale(value: number, scale: number): number
  static removeDrawingScale(value: Point, scale: number): Point
  static removeDrawingScale(value: number | Point, scale: number): number | Point {
    if (scale <= 0) {
      throw new Error('Invalid drawing scale: must be positive')
    }

    if (typeof value === 'number') {
      return value * scale
    } else {
      return {
        x: value.x * scale,
        y: value.y * scale
      }
    }
  }

  /**
   * Convert circle to ellipse parameters (for Illustrator's coordinate system)
   * @param center - Circle center point
   * @param radius - Circle radius
   * @returns Ellipse positioning params
   */
  static circleToEllipseParams(center: Point, radius: number): EllipseParams {
    return {
      left: center.x - radius,
      top: center.y + radius, // Illustrator Y increases upward
      width: radius * 2,
      height: radius * 2
    }
  }

  /**
   * Convert ellipse parameters back to circle center
   * @param params - Ellipse positioning parameters
   * @returns Circle center point
   */
  static ellipseParamsToCircleCenter(params: EllipseParams): Point {
    const radius = params.width / 2

    return {
      x: params.left + radius,
      y: params.top - radius // Illustrator Y increases upward
    }
  }
}
