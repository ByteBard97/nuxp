import { describe, it, expect } from 'vitest'
import { CoordinateSystemManager as CSM, type Bounds, type Point } from './CoordinateSystemManager'

describe('CoordinateSystemManager', () => {
  // -------------------------------------------------------------------------
  // getBounds
  // -------------------------------------------------------------------------
  describe('getBounds', () => {
    it('returns correct min/max from a point array', () => {
      const points: Point[] = [
        { x: 1, y: 5 },
        { x: 3, y: 2 },
        { x: -1, y: 8 },
      ]
      const bounds = CSM.getBounds(points)
      expect(bounds).toEqual({ minX: -1, maxX: 3, minY: 2, maxY: 8 })
    })

    it('handles a single point', () => {
      const bounds = CSM.getBounds([{ x: 7, y: 3 }])
      expect(bounds).toEqual({ minX: 7, maxX: 7, minY: 3, maxY: 3 })
    })

    it('throws on empty array', () => {
      expect(() => CSM.getBounds([])).toThrow('Cannot calculate bounds for empty point array')
    })
  })

  // -------------------------------------------------------------------------
  // getBoundsDimensions
  // -------------------------------------------------------------------------
  describe('getBoundsDimensions', () => {
    it('returns correct width and height', () => {
      const bounds: Bounds = { minX: 2, maxX: 10, minY: 3, maxY: 15 }
      expect(CSM.getBoundsDimensions(bounds)).toEqual({ width: 8, height: 12 })
    })

    it('returns zero dimensions for a point-like bounds', () => {
      const bounds: Bounds = { minX: 5, maxX: 5, minY: 5, maxY: 5 }
      expect(CSM.getBoundsDimensions(bounds)).toEqual({ width: 0, height: 0 })
    })
  })

  // -------------------------------------------------------------------------
  // getBoundsCenter
  // -------------------------------------------------------------------------
  describe('getBoundsCenter', () => {
    it('returns the center point', () => {
      const bounds: Bounds = { minX: 0, maxX: 10, minY: 0, maxY: 20 }
      expect(CSM.getBoundsCenter(bounds)).toEqual({ x: 5, y: 10 })
    })

    it('works with negative bounds', () => {
      const bounds: Bounds = { minX: -10, maxX: 10, minY: -4, maxY: 6 }
      expect(CSM.getBoundsCenter(bounds)).toEqual({ x: 0, y: 1 })
    })
  })

  // -------------------------------------------------------------------------
  // translatePoints / translatePoint
  // -------------------------------------------------------------------------
  describe('translatePoints', () => {
    it('offsets all points by the given vector', () => {
      const points: Point[] = [
        { x: 1, y: 2 },
        { x: 3, y: 4 },
      ]
      const result = CSM.translatePoints(points, { x: 10, y: -5 })
      expect(result).toEqual([
        { x: 11, y: -3 },
        { x: 13, y: -1 },
      ])
    })
  })

  describe('translatePoint', () => {
    it('offsets a single point', () => {
      expect(CSM.translatePoint({ x: 5, y: 3 }, { x: -2, y: 7 })).toEqual({ x: 3, y: 10 })
    })
  })

  // -------------------------------------------------------------------------
  // scalePoints
  // -------------------------------------------------------------------------
  describe('scalePoints', () => {
    it('multiplies all coordinates by the scale factor', () => {
      const points: Point[] = [
        { x: 2, y: 3 },
        { x: -1, y: 4 },
      ]
      expect(CSM.scalePoints(points, 3)).toEqual([
        { x: 6, y: 9 },
        { x: -3, y: 12 },
      ])
    })
  })

  // -------------------------------------------------------------------------
  // distance
  // -------------------------------------------------------------------------
  describe('distance', () => {
    it('computes Euclidean distance (3-4-5 triangle)', () => {
      expect(CSM.distance({ x: 0, y: 0 }, { x: 3, y: 4 })).toBe(5)
    })

    it('returns 0 for identical points', () => {
      expect(CSM.distance({ x: 7, y: 7 }, { x: 7, y: 7 })).toBe(0)
    })
  })

  // -------------------------------------------------------------------------
  // midpoint
  // -------------------------------------------------------------------------
  describe('midpoint', () => {
    it('returns the average of two points', () => {
      expect(CSM.midpoint({ x: 0, y: 0 }, { x: 10, y: 6 })).toEqual({ x: 5, y: 3 })
    })
  })

  // -------------------------------------------------------------------------
  // circlesOverlap
  // -------------------------------------------------------------------------
  describe('circlesOverlap', () => {
    it('returns true when circles overlap', () => {
      expect(CSM.circlesOverlap({ x: 0, y: 0 }, 5, { x: 8, y: 0 }, 5)).toBe(true)
    })

    it('returns false when circles are separated', () => {
      expect(CSM.circlesOverlap({ x: 0, y: 0 }, 3, { x: 10, y: 0 }, 3)).toBe(false)
    })

    it('returns false when circles are exactly touching (strictly less-than)', () => {
      expect(CSM.circlesOverlap({ x: 0, y: 0 }, 5, { x: 10, y: 0 }, 5)).toBe(false)
    })
  })

  // -------------------------------------------------------------------------
  // d3ToIllustrator / illustratorToD3
  // -------------------------------------------------------------------------
  describe('d3ToIllustrator', () => {
    it('flips y-axis using artboard height', () => {
      const result = CSM.d3ToIllustrator({ x: 10, y: 30 }, 100)
      expect(result).toEqual({ x: 10, y: 70 })
    })
  })

  describe('illustratorToD3', () => {
    it('flips y-axis using artboard height', () => {
      const result = CSM.illustratorToD3({ x: 10, y: 70 }, 100)
      expect(result).toEqual({ x: 10, y: 30 })
    })

    it('round-trips with d3ToIllustrator', () => {
      const original: Point = { x: 42, y: 17 }
      const height = 200
      const converted = CSM.d3ToIllustrator(original, height)
      const roundTripped = CSM.illustratorToD3(converted, height)
      expect(roundTripped).toEqual(original)
    })
  })

  // -------------------------------------------------------------------------
  // validateCircleInBounds
  // -------------------------------------------------------------------------
  describe('validateCircleInBounds', () => {
    const bounds: Bounds = { minX: 0, maxX: 100, minY: 0, maxY: 100 }

    it('does not throw when the circle is inside', () => {
      expect(() => CSM.validateCircleInBounds({ x: 50, y: 50 }, 10, bounds)).not.toThrow()
    })

    it('throws when circle extends beyond the LEFT edge', () => {
      expect(() => CSM.validateCircleInBounds({ x: 3, y: 50 }, 10, bounds)).toThrow('LEFT')
    })

    it('throws when circle extends beyond the RIGHT edge', () => {
      expect(() => CSM.validateCircleInBounds({ x: 97, y: 50 }, 10, bounds)).toThrow('RIGHT')
    })

    it('throws when circle extends beyond the TOP edge', () => {
      expect(() => CSM.validateCircleInBounds({ x: 50, y: 3 }, 10, bounds)).toThrow('TOP')
    })

    it('throws when circle extends beyond the BOTTOM edge', () => {
      expect(() => CSM.validateCircleInBounds({ x: 50, y: 97 }, 10, bounds)).toThrow('BOTTOM')
    })

    it('includes the custom label in the error message', () => {
      expect(() => CSM.validateCircleInBounds({ x: 0, y: 50 }, 10, bounds, 'TreeCircle')).toThrow(
        'TreeCircle',
      )
    })
  })

  // -------------------------------------------------------------------------
  // validateBounds
  // -------------------------------------------------------------------------
  describe('validateBounds', () => {
    it('does not throw for valid bounds', () => {
      expect(() => CSM.validateBounds({ minX: 0, maxX: 10, minY: 0, maxY: 10 })).not.toThrow()
    })

    it('throws when minX > maxX', () => {
      expect(() => CSM.validateBounds({ minX: 10, maxX: 0, minY: 0, maxY: 10 })).toThrow(
        'minX',
      )
    })

    it('throws when minY > maxY', () => {
      expect(() => CSM.validateBounds({ minX: 0, maxX: 10, minY: 10, maxY: 0 })).toThrow(
        'minY',
      )
    })
  })

  // -------------------------------------------------------------------------
  // insetBounds
  // -------------------------------------------------------------------------
  describe('insetBounds', () => {
    it('shrinks bounds by the inset value', () => {
      const bounds: Bounds = { minX: 0, maxX: 100, minY: 0, maxY: 100 }
      expect(CSM.insetBounds(bounds, 10)).toEqual({ minX: 10, maxX: 90, minY: 10, maxY: 90 })
    })

    it('throws when inset would create invalid bounds', () => {
      const bounds: Bounds = { minX: 0, maxX: 10, minY: 0, maxY: 10 }
      expect(() => CSM.insetBounds(bounds, 20)).toThrow()
    })
  })

  // -------------------------------------------------------------------------
  // rotatePoints
  // -------------------------------------------------------------------------
  describe('rotatePoints', () => {
    it('rotates a point 90 degrees counterclockwise around the origin', () => {
      const points: Point[] = [{ x: 1, y: 0 }]
      const result = CSM.rotatePoints(points, 90, { x: 0, y: 0 })
      expect(result[0].x).toBeCloseTo(0)
      expect(result[0].y).toBeCloseTo(1)
    })

    it('rotates 180 degrees', () => {
      const points: Point[] = [{ x: 5, y: 0 }]
      const result = CSM.rotatePoints(points, 180, { x: 0, y: 0 })
      expect(result[0].x).toBeCloseTo(-5)
      expect(result[0].y).toBeCloseTo(0)
    })

    it('rotates around a non-origin center', () => {
      const points: Point[] = [{ x: 10, y: 5 }]
      const result = CSM.rotatePoints(points, 90, { x: 5, y: 5 })
      expect(result[0].x).toBeCloseTo(5)
      expect(result[0].y).toBeCloseTo(10)
    })
  })

  // -------------------------------------------------------------------------
  // calculatePolygonArea
  // -------------------------------------------------------------------------
  describe('calculatePolygonArea', () => {
    it('returns the area of a known right triangle', () => {
      // Triangle with base 4 and height 3 => area = 6
      const triangle: Point[] = [
        { x: 0, y: 0 },
        { x: 4, y: 0 },
        { x: 0, y: 3 },
      ]
      expect(CSM.calculatePolygonArea(triangle)).toBeCloseTo(6)
    })

    it('returns 0 for fewer than 3 points', () => {
      expect(CSM.calculatePolygonArea([{ x: 0, y: 0 }, { x: 1, y: 1 }])).toBe(0)
      expect(CSM.calculatePolygonArea([])).toBe(0)
    })

    it('computes area of a unit square', () => {
      const square: Point[] = [
        { x: 0, y: 0 },
        { x: 1, y: 0 },
        { x: 1, y: 1 },
        { x: 0, y: 1 },
      ]
      expect(CSM.calculatePolygonArea(square)).toBeCloseTo(1)
    })
  })

  // -------------------------------------------------------------------------
  // calculatePolygonCentroid
  // -------------------------------------------------------------------------
  describe('calculatePolygonCentroid', () => {
    it('computes centroid of a symmetric square', () => {
      const square: Point[] = [
        { x: 0, y: 0 },
        { x: 10, y: 0 },
        { x: 10, y: 10 },
        { x: 0, y: 10 },
      ]
      const centroid = CSM.calculatePolygonCentroid(square)
      expect(centroid.x).toBeCloseTo(5)
      expect(centroid.y).toBeCloseTo(5)
    })

    it('returns (0,0) for fewer than 3 points', () => {
      expect(CSM.calculatePolygonCentroid([])).toEqual({ x: 0, y: 0 })
    })
  })

  // -------------------------------------------------------------------------
  // isPointInBounds
  // -------------------------------------------------------------------------
  describe('isPointInBounds', () => {
    const bounds: Bounds = { minX: 0, maxX: 10, minY: 0, maxY: 10 }

    it('returns true for a point inside', () => {
      expect(CSM.isPointInBounds({ x: 5, y: 5 }, bounds)).toBe(true)
    })

    it('returns true for a point on the boundary', () => {
      expect(CSM.isPointInBounds({ x: 0, y: 0 }, bounds)).toBe(true)
      expect(CSM.isPointInBounds({ x: 10, y: 10 }, bounds)).toBe(true)
    })

    it('returns false for a point outside', () => {
      expect(CSM.isPointInBounds({ x: -1, y: 5 }, bounds)).toBe(false)
      expect(CSM.isPointInBounds({ x: 5, y: 11 }, bounds)).toBe(false)
    })
  })

  // -------------------------------------------------------------------------
  // boundsIntersect
  // -------------------------------------------------------------------------
  describe('boundsIntersect', () => {
    it('returns true for overlapping bounds', () => {
      const a: Bounds = { minX: 0, maxX: 10, minY: 0, maxY: 10 }
      const b: Bounds = { minX: 5, maxX: 15, minY: 5, maxY: 15 }
      expect(CSM.boundsIntersect(a, b)).toBe(true)
    })

    it('returns false for separated bounds', () => {
      const a: Bounds = { minX: 0, maxX: 5, minY: 0, maxY: 5 }
      const b: Bounds = { minX: 10, maxX: 15, minY: 10, maxY: 15 }
      expect(CSM.boundsIntersect(a, b)).toBe(false)
    })

    it('returns true for touching bounds', () => {
      const a: Bounds = { minX: 0, maxX: 5, minY: 0, maxY: 5 }
      const b: Bounds = { minX: 5, maxX: 10, minY: 0, maxY: 5 }
      expect(CSM.boundsIntersect(a, b)).toBe(true)
    })
  })

  // -------------------------------------------------------------------------
  // expandBounds
  // -------------------------------------------------------------------------
  describe('expandBounds', () => {
    it('grows bounds outward by the given margin', () => {
      const bounds: Bounds = { minX: 10, maxX: 20, minY: 10, maxY: 20 }
      expect(CSM.expandBounds(bounds, 5)).toEqual({ minX: 5, maxX: 25, minY: 5, maxY: 25 })
    })
  })

  // -------------------------------------------------------------------------
  // applyDrawingScale / removeDrawingScale
  // -------------------------------------------------------------------------
  describe('applyDrawingScale', () => {
    it('divides a number by the scale', () => {
      expect(CSM.applyDrawingScale(120, 120)).toBe(1)
    })

    it('divides a Point by the scale', () => {
      const result = CSM.applyDrawingScale({ x: 240, y: 360 }, 120)
      expect(result).toEqual({ x: 2, y: 3 })
    })

    it('throws on non-positive scale', () => {
      expect(() => CSM.applyDrawingScale(10, 0)).toThrow('must be positive')
      expect(() => CSM.applyDrawingScale(10, -1)).toThrow('must be positive')
    })
  })

  describe('removeDrawingScale', () => {
    it('multiplies a number by the scale', () => {
      expect(CSM.removeDrawingScale(1, 120)).toBe(120)
    })

    it('multiplies a Point by the scale', () => {
      const result = CSM.removeDrawingScale({ x: 2, y: 3 }, 120)
      expect(result).toEqual({ x: 240, y: 360 })
    })

    it('throws on non-positive scale', () => {
      expect(() => CSM.removeDrawingScale(10, 0)).toThrow('must be positive')
    })

    it('round-trips with applyDrawingScale for number', () => {
      const original = 50
      const scale = 48
      expect(CSM.removeDrawingScale(CSM.applyDrawingScale(original, scale), scale)).toBeCloseTo(
        original,
      )
    })

    it('round-trips with applyDrawingScale for Point', () => {
      const original: Point = { x: 100, y: 200 }
      const scale = 48
      const result = CSM.removeDrawingScale(
        CSM.applyDrawingScale(original, scale) as Point,
        scale,
      ) as Point
      expect(result.x).toBeCloseTo(original.x)
      expect(result.y).toBeCloseTo(original.y)
    })
  })

  // -------------------------------------------------------------------------
  // circleToEllipseParams / ellipseParamsToCircleCenter
  // -------------------------------------------------------------------------
  describe('circleToEllipseParams', () => {
    it('converts circle to ellipse params', () => {
      const result = CSM.circleToEllipseParams({ x: 10, y: 20 }, 5)
      expect(result).toEqual({ left: 5, top: 25, width: 10, height: 10 })
    })
  })

  describe('ellipseParamsToCircleCenter', () => {
    it('converts ellipse params back to circle center', () => {
      const result = CSM.ellipseParamsToCircleCenter({ left: 5, top: 25, width: 10, height: 10 })
      expect(result).toEqual({ x: 10, y: 20 })
    })

    it('round-trips with circleToEllipseParams', () => {
      const center: Point = { x: 42, y: 17 }
      const radius = 8
      const ellipse = CSM.circleToEllipseParams(center, radius)
      const recovered = CSM.ellipseParamsToCircleCenter(ellipse)
      expect(recovered).toEqual(center)
    })
  })

  // -------------------------------------------------------------------------
  // Unit conversions
  // -------------------------------------------------------------------------
  describe('unit conversions', () => {
    it('inchesToPoints: 1 inch = 72 points', () => {
      expect(CSM.inchesToPoints(1)).toBe(72)
      expect(CSM.inchesToPoints(2.5)).toBe(180)
    })

    it('pointsToInches: 72 points = 1 inch', () => {
      expect(CSM.pointsToInches(72)).toBe(1)
    })

    it('feetToInches: 1 foot = 12 inches', () => {
      expect(CSM.feetToInches(1)).toBe(12)
    })

    it('inchesToFeet: 12 inches = 1 foot', () => {
      expect(CSM.inchesToFeet(12)).toBe(1)
    })
  })
})
