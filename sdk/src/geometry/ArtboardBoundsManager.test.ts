import { describe, it, expect } from 'vitest'
import { ArtboardBoundsManager } from './ArtboardBoundsManager'

describe('ArtboardBoundsManager', () => {
  // -------------------------------------------------------------------------
  // constructor / getAvailableBounds
  // -------------------------------------------------------------------------
  describe('getAvailableBounds', () => {
    it('returns artboard bounds inset by the default margin (0.5)', () => {
      const mgr = new ArtboardBoundsManager(24, 36)
      const bounds = mgr.getAvailableBounds()
      expect(bounds).toEqual({ minX: 0.5, maxX: 23.5, minY: 0.5, maxY: 35.5 })
    })

    it('respects a custom margin', () => {
      const mgr = new ArtboardBoundsManager(10, 10, 1)
      const bounds = mgr.getAvailableBounds()
      expect(bounds).toEqual({ minX: 1, maxX: 9, minY: 1, maxY: 9 })
    })

    it('handles zero margin', () => {
      const mgr = new ArtboardBoundsManager(20, 30, 0)
      const bounds = mgr.getAvailableBounds()
      expect(bounds).toEqual({ minX: 0, maxX: 20, minY: 0, maxY: 30 })
    })
  })

  // -------------------------------------------------------------------------
  // getCenter
  // -------------------------------------------------------------------------
  describe('getCenter', () => {
    it('returns the center of the available area', () => {
      const mgr = new ArtboardBoundsManager(20, 30, 0)
      expect(mgr.getCenter()).toEqual({ x: 10, y: 15 })
    })

    it('accounts for margin when computing center', () => {
      const mgr = new ArtboardBoundsManager(20, 30, 5)
      // Available: minX=5, maxX=15, minY=5, maxY=25 => center (10, 15)
      expect(mgr.getCenter()).toEqual({ x: 10, y: 15 })
    })
  })

  // -------------------------------------------------------------------------
  // assertCircleFits
  // -------------------------------------------------------------------------
  describe('assertCircleFits', () => {
    const mgr = new ArtboardBoundsManager(20, 20, 1)
    // Available: {minX:1, maxX:19, minY:1, maxY:19}

    it('does not throw when the circle fits inside', () => {
      expect(() => mgr.assertCircleFits({ x: 10, y: 10 }, 5)).not.toThrow()
    })

    it('throws when the circle exceeds the left edge', () => {
      expect(() => mgr.assertCircleFits({ x: 2, y: 10 }, 5)).toThrow('exceeds artboard bounds')
    })

    it('throws when the circle exceeds the right edge', () => {
      expect(() => mgr.assertCircleFits({ x: 18, y: 10 }, 5)).toThrow('exceeds artboard bounds')
    })

    it('throws when the circle exceeds the top edge', () => {
      expect(() => mgr.assertCircleFits({ x: 10, y: 2 }, 5)).toThrow('exceeds artboard bounds')
    })

    it('throws when the circle exceeds the bottom edge', () => {
      expect(() => mgr.assertCircleFits({ x: 10, y: 18 }, 5)).toThrow('exceeds artboard bounds')
    })

    it('allows a circle that exactly fills the available space', () => {
      // Available bounds 1..19 (width=height=18), radius=9, center at 10,10
      expect(() => mgr.assertCircleFits({ x: 10, y: 10 }, 9)).not.toThrow()
    })
  })

  // -------------------------------------------------------------------------
  // clampPoint
  // -------------------------------------------------------------------------
  describe('clampPoint', () => {
    const mgr = new ArtboardBoundsManager(20, 20, 0)
    // Available: {minX:0, maxX:20, minY:0, maxY:20}

    it('returns the point unchanged when it is inside bounds', () => {
      expect(mgr.clampPoint({ x: 10, y: 10 })).toEqual({ x: 10, y: 10 })
    })

    it('clamps a point below the minimum', () => {
      expect(mgr.clampPoint({ x: -5, y: -3 })).toEqual({ x: 0, y: 0 })
    })

    it('clamps a point above the maximum', () => {
      expect(mgr.clampPoint({ x: 25, y: 30 })).toEqual({ x: 20, y: 20 })
    })

    it('accounts for radius when clamping', () => {
      expect(mgr.clampPoint({ x: 1, y: 1 }, 5)).toEqual({ x: 5, y: 5 })
      expect(mgr.clampPoint({ x: 19, y: 19 }, 5)).toEqual({ x: 15, y: 15 })
    })
  })
})
