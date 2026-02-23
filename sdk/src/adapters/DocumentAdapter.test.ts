import { describe, it, expect, vi } from 'vitest'
import {
  getDocumentInfo,
  getUnits,
  setUnits,
  getAvailableFonts,
  getArtboardInfo,
} from './DocumentAdapter'
import type { BridgeCallFn } from '../primitives/types'

function createMockBridge(returnValue: unknown = {}): BridgeCallFn {
  return vi.fn().mockResolvedValue(returnValue) as unknown as BridgeCallFn
}

describe('DocumentAdapter', () => {
  // -------------------------------------------------------------------------
  // getDocumentInfo
  // -------------------------------------------------------------------------
  describe('getDocumentInfo', () => {
    it('calls bridge with "getDocumentInfo"', async () => {
      const info = {
        hasDocument: true,
        name: 'test',
        fullName: 'test.ai',
        path: '/tmp',
        filePath: '/tmp/test.ai',
        saved: true,
        artboardCount: 1,
        currentArtboard: 0,
        bounds: { left: 0, top: 0, right: 100, bottom: 100 },
        width: 100,
        height: 100,
        widthPoints: 7200,
        heightPoints: 7200,
      }
      const bridge = createMockBridge(info)
      const result = await getDocumentInfo(bridge)

      expect(bridge).toHaveBeenCalledWith('getDocumentInfo')
      expect(result).toEqual(info)
    })
  })

  // -------------------------------------------------------------------------
  // getUnits
  // -------------------------------------------------------------------------
  describe('getUnits', () => {
    it('calls bridge with "getDocumentUnits"', async () => {
      const units = { units: 2, unitsName: 'inches' as const }
      const bridge = createMockBridge(units)
      const result = await getUnits(bridge)

      expect(bridge).toHaveBeenCalledWith('getDocumentUnits')
      expect(result).toEqual(units)
    })
  })

  // -------------------------------------------------------------------------
  // setUnits
  // -------------------------------------------------------------------------
  describe('setUnits', () => {
    it('calls bridge with "setDocumentUnits" and the unit name', async () => {
      const response = { units: 3, unitsName: 'millimeters' as const }
      const bridge = createMockBridge(response)
      const result = await setUnits(bridge, 'millimeters')

      expect(bridge).toHaveBeenCalledWith('setDocumentUnits', 'millimeters')
      expect(result).toEqual(response)
    })
  })

  // -------------------------------------------------------------------------
  // getAvailableFonts
  // -------------------------------------------------------------------------
  describe('getAvailableFonts', () => {
    it('calls bridge with "getAvailableFonts"', async () => {
      const fonts = {
        success: true,
        fonts: [{ name: 'Arial', family: 'Arial', style: 'Regular' }],
        count: 1,
      }
      const bridge = createMockBridge(fonts)
      const result = await getAvailableFonts(bridge)

      expect(bridge).toHaveBeenCalledWith('getAvailableFonts')
      expect(result).toEqual(fonts)
    })
  })

  // -------------------------------------------------------------------------
  // getArtboardInfo
  // -------------------------------------------------------------------------
  describe('getArtboardInfo', () => {
    it('calls bridge with "getArtboardInfo"', async () => {
      const info = {
        left: 0,
        top: 0,
        right: 792,
        bottom: 612,
        artboardWidth: 792,
        artboardHeight: 612,
      }
      const bridge = createMockBridge(info)
      const result = await getArtboardInfo(bridge)

      expect(bridge).toHaveBeenCalledWith('getArtboardInfo')
      expect(result).toEqual(info)
    })
  })
})
