import { describe, it, expect, vi } from 'vitest'
import { SvgPlacementService, type PlacementResult } from './SvgPlacementService'

describe('SvgPlacementService', () => {
  // -------------------------------------------------------------------------
  // placeWithSvg
  // -------------------------------------------------------------------------

  describe('placeWithSvg', () => {
    it('calls bridge with correct arguments and returns PlacementResult', async () => {
      const expected: PlacementResult = {
        uuid: 'abc-123',
        bounds: { left: 10, top: 20, width: 50, height: 50 },
      }
      const mockBridge = vi.fn().mockResolvedValue(expected)
      const svc = new SvgPlacementService(mockBridge)

      const result = await svc.placeWithSvg({
        symbolName: 'circle-sym',
        position: { x: 100, y: 200 },
        radiusPts: 25,
        color: '#ff0000',
        label: 'R1',
        displayLabel: 'Resistor 1',
        opacity: 80,
        fontName: 'Helvetica',
        fontSize: 14,
        svgPath: '/tmp/icon.svg',
        svgScale: 0.5,
        svgOffsetX: 2,
        svgOffsetY: 3,
        circleFillStyle: 'outline',
        strokeEnabled: false,
        strokeColor: '#00ff00',
        strokeWidth: 2.0,
      })

      expect(result).toEqual(expected)
      expect(mockBridge).toHaveBeenCalledWith(
        'placeWithSvg',
        'circle-sym',
        100,
        200,
        25,
        '#ff0000',
        'R1',
        'Resistor 1',
        80,
        'Helvetica',
        14,
        '/tmp/icon.svg',
        0.5,
        2,
        3,
        'outline',
        false,
        '#00ff00',
        2.0
      )
    })

    it('applies default values for optional parameters', async () => {
      const mockBridge = vi.fn().mockResolvedValue({ uuid: 'x', bounds: { left: 0, top: 0, width: 1, height: 1 } })
      const svc = new SvgPlacementService(mockBridge)

      await svc.placeWithSvg({
        symbolName: 'sym',
        position: { x: 0, y: 0 },
        radiusPts: 10,
        color: '#000',
      })

      expect(mockBridge).toHaveBeenCalledWith(
        'placeWithSvg',
        'sym',
        0,
        0,
        10,
        '#000',
        '',        // label defaults to ''
        '',        // displayLabel defaults to label ?? ''
        100,       // opacity
        'Arial',   // fontName
        12,        // fontSize
        '',        // svgPath
        1.0,       // svgScale
        0,         // svgOffsetX
        0,         // svgOffsetY
        'filled',  // circleFillStyle
        true,      // strokeEnabled
        '#000000', // strokeColor
        1.0        // strokeWidth
      )
    })
  })

  // -------------------------------------------------------------------------
  // validateSvgContent
  // -------------------------------------------------------------------------

  describe('validateSvgContent', () => {
    const svc = new SvgPlacementService(vi.fn())

    it('returns invalid for empty string', () => {
      const r = svc.validateSvgContent('')
      expect(r.valid).toBe(false)
      expect(r.errors).toContain('SVG content is empty')
    })

    it('returns invalid for whitespace-only string', () => {
      const r = svc.validateSvgContent('   ')
      expect(r.valid).toBe(false)
      expect(r.errors).toContain('SVG content is empty')
    })

    it('returns invalid when no <svg element present', () => {
      const r = svc.validateSvgContent('<div>not svg</div>')
      expect(r.valid).toBe(false)
      expect(r.errors).toContain('Content does not contain SVG root element')
    })

    it('valid SVG with viewBox extracts dimensions', () => {
      const svg = '<svg viewBox="0 0 100 200"><rect/></svg>'
      const r = svc.validateSvgContent(svg)
      expect(r.valid).toBe(true)
      expect(r.hasViewBox).toBe(true)
      expect(r.width).toBe(100)
      expect(r.height).toBe(200)
      expect(r.errors).toHaveLength(0)
    })

    it('warns when viewBox is missing', () => {
      const svg = '<svg width="50" height="60"><rect/></svg>'
      const r = svc.validateSvgContent(svg)
      expect(r.valid).toBe(true)
      expect(r.hasViewBox).toBe(false)
      expect(r.warnings.some(w => w.includes('viewBox'))).toBe(true)
      // Falls back to width/height attributes
      expect(r.width).toBe(50)
      expect(r.height).toBe(60)
    })

    it('warns when no viewBox and no dimensions', () => {
      const svg = '<svg><rect/></svg>'
      const r = svc.validateSvgContent(svg)
      expect(r.valid).toBe(true)
      expect(r.warnings.some(w => w.includes('sizing may be unpredictable'))).toBe(true)
    })

    it('warns on <script> elements', () => {
      const svg = '<svg viewBox="0 0 10 10"><script>alert(1)</script></svg>'
      const r = svc.validateSvgContent(svg)
      expect(r.warnings.some(w => w.includes('script'))).toBe(true)
    })

    it('warns on <foreignObject> elements', () => {
      const svg = '<svg viewBox="0 0 10 10"><foreignObject></foreignObject></svg>'
      const r = svc.validateSvgContent(svg)
      expect(r.warnings.some(w => w.includes('foreignObject'))).toBe(true)
    })

    it('warns on embedded images', () => {
      const svg = '<svg viewBox="0 0 10 10"><image xlink:href="data:image/png;base64,..." /></svg>'
      const r = svc.validateSvgContent(svg)
      expect(r.warnings.some(w => w.includes('embedded images'))).toBe(true)
    })

    it('warns on <filter> elements', () => {
      const svg = '<svg viewBox="0 0 10 10"><filter id="blur"><feGaussianBlur/></filter></svg>'
      const r = svc.validateSvgContent(svg)
      expect(r.warnings.some(w => w.includes('filters'))).toBe(true)
    })
  })
})
