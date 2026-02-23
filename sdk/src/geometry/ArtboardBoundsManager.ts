/**
 * ArtboardBoundsManager
 *
 * High-level manager that knows about the current artboard and enforces bounds.
 * Uses CoordinateSystemManager for low-level operations.
 */

import { CoordinateSystemManager as CSM, type Point, type Bounds } from './CoordinateSystemManager'

export class ArtboardBoundsManager {
  private artboardBounds: Bounds
  private margin: number

  constructor(artboardWidthInches: number, artboardHeightInches: number, marginInches = 0.5) {
    this.artboardBounds = {
      minX: 0,
      maxX: artboardWidthInches,
      minY: 0,
      maxY: artboardHeightInches
    }
    this.margin = marginInches
  }

  /**
   * Get available bounds (artboard minus margins)
   */
  getAvailableBounds(): Bounds {
    return CSM.insetBounds(this.artboardBounds, this.margin)
  }

  /**
   * Get center of available area
   */
  getCenter(): Point {
    return CSM.getBoundsCenter(this.getAvailableBounds())
  }

  /**
   * Check if a circle fits within bounds (including margin)
   * @throws Error if circle is outside bounds
   */
  assertCircleFits(center: Point, radiusInches: number): void {
    const available = this.getAvailableBounds()
    const circleBounds = CSM.getCircleBounds(center, radiusInches)

    if (circleBounds.minX < available.minX ||
        circleBounds.maxX > available.maxX ||
        circleBounds.minY < available.minY ||
        circleBounds.maxY > available.maxY) {

      throw new Error(
        `Circle at (${center.x.toFixed(2)}", ${center.y.toFixed(2)}") with radius ${radiusInches.toFixed(2)}" ` +
        `exceeds artboard bounds (${available.minX.toFixed(2)}", ${available.minY.toFixed(2)}) to ` +
        `(${available.maxX.toFixed(2)}", ${available.maxY.toFixed(2)}")`
      )
    }
  }

  /**
   * Clamp a point to stay within available bounds
   */
  clampPoint(point: Point, radiusInches = 0): Point {
    const available = this.getAvailableBounds()
    return {
      x: Math.max(available.minX + radiusInches, Math.min(available.maxX - radiusInches, point.x)),
      y: Math.max(available.minY + radiusInches, Math.min(available.maxY - radiusInches, point.y))
    }
  }
}
