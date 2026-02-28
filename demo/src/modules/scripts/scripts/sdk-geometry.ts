import type { Script } from '../types'
import { CoordinateSystemManager } from '@nuxp/sdk'

const sdkGeometry: Script = {
  id: 'sdk-geometry',
  name: 'CoordinateSystemManager',
  description: 'Bounds calculation, point transforms, distance, containment, and unit conversions',
  category: 'sdk',
  async run() {
    const CSM = CoordinateSystemManager

    // ── Bounds from points ──────────────────────────────────────────────
    const points = [
      { x: 10, y: 20 },
      { x: 50, y: 80 },
      { x: 30, y: 5 },
      { x: 90, y: 60 },
    ]
    const bounds = CSM.getBounds(points)
    const dimensions = CSM.getBoundsDimensions(bounds)
    const center = CSM.getBoundsCenter(bounds)

    // ── Point transforms ────────────────────────────────────────────────
    const translated = CSM.translatePoints(points, { x: 100, y: 50 })
    const scaled = CSM.scalePoints(points, 2)
    const rotated = CSM.rotatePoints(points, 45, center)

    // ── Distance and midpoint ───────────────────────────────────────────
    const p1 = { x: 0, y: 0 }
    const p2 = { x: 3, y: 4 }
    const dist = CSM.distance(p1, p2) // 5 (3-4-5 triangle)
    const mid = CSM.midpoint(p1, p2)

    // ── Containment checks ──────────────────────────────────────────────
    const testBounds = { minX: 0, maxX: 100, minY: 0, maxY: 100 }
    const insidePoint = { x: 50, y: 50 }
    const outsidePoint = { x: 150, y: 50 }

    // ── Unit conversions ────────────────────────────────────────────────
    const oneInch = CSM.inchesToPoints(1)    // 72 pt
    const twoFeet = CSM.feetToInches(2)      // 24 in
    const backToInches = CSM.pointsToInches(144) // 2 in

    // ── Polygon area ────────────────────────────────────────────────────
    const square = [
      { x: 0, y: 0 },
      { x: 100, y: 0 },
      { x: 100, y: 100 },
      { x: 0, y: 100 },
    ]
    const area = CSM.calculatePolygonArea(square)
    const centroid = CSM.calculatePolygonCentroid(square)

    // Round floats for display
    const round2 = (n: number) => Math.round(n * 100) / 100
    const roundPt = (p: { x: number; y: number }) => ({ x: round2(p.x), y: round2(p.y) })

    return {
      success: true,
      message: `Bounds: ${dimensions.width}x${dimensions.height}, distance(0,0 \u2192 3,4) = ${dist}`,
      data: {
        bounds: { bounds, dimensions, center: roundPt(center) },
        transforms: {
          translated: translated.map(roundPt),
          scaled: scaled.map(roundPt),
          rotated: rotated.map(roundPt),
        },
        geometry: {
          distance: dist,
          midpoint: mid,
          polygonArea: area,
          polygonCentroid: roundPt(centroid),
        },
        containment: {
          insideCheck: CSM.isPointInBounds(insidePoint, testBounds),
          outsideCheck: CSM.isPointInBounds(outsidePoint, testBounds),
        },
        unitConversions: {
          '1 inch in points': oneInch,
          '2 feet in inches': twoFeet,
          '144 points in inches': backToInches,
        },
      },
    }
  },
}

export default sdkGeometry
