import { describe, it, expect, vi } from 'vitest'
import {
  startPlacementPreview,
  updatePlacementPreview,
  finishPlacement,
  cancelPlacement,
  placeSymbolInstance,
  setItemMetadata,
  importAsset,
} from './PlacementAdapter'
import type { BridgeCallFn } from '../primitives/types'

function createMockBridge(returnValue: unknown = undefined): BridgeCallFn {
  return vi.fn().mockResolvedValue(returnValue) as unknown as BridgeCallFn
}

describe('PlacementAdapter', () => {
  // -------------------------------------------------------------------------
  // startPlacementPreview
  // -------------------------------------------------------------------------
  describe('startPlacementPreview', () => {
    it('calls bridge with symbolName, radiusPts, and color', async () => {
      const bridge = createMockBridge()
      await startPlacementPreview(bridge, 'TreeSymbol', 24, '#00FF00')

      expect(bridge).toHaveBeenCalledWith('startPlacementPreview', 'TreeSymbol', 24, '#00FF00')
    })
  })

  // -------------------------------------------------------------------------
  // updatePlacementPreview
  // -------------------------------------------------------------------------
  describe('updatePlacementPreview', () => {
    it('calls bridge with x and y coordinates', async () => {
      const bridge = createMockBridge()
      await updatePlacementPreview(bridge, { x: 100, y: 200 })

      expect(bridge).toHaveBeenCalledWith('updatePlacementPreview', 100, 200)
    })
  })

  // -------------------------------------------------------------------------
  // finishPlacement
  // -------------------------------------------------------------------------
  describe('finishPlacement', () => {
    it('calls bridge with symbolName, scale, and rotation and returns PlacementResult', async () => {
      const placementResult = {
        success: true,
        uuid: 'abc-123',
        bounds: { left: 10, top: 20, right: 30, bottom: 40 },
      }
      const bridge = createMockBridge(placementResult)
      const result = await finishPlacement(bridge, 'TreeSymbol', 1.5, 45)

      expect(bridge).toHaveBeenCalledWith('finishPlacement', 'TreeSymbol', 1.5, 45)
      expect(result).toEqual(placementResult)
    })

    it('uses default scale and rotation when not provided', async () => {
      const bridge = createMockBridge({ success: true, uuid: 'x' })
      await finishPlacement(bridge, 'Shrub')

      expect(bridge).toHaveBeenCalledWith('finishPlacement', 'Shrub', 1.0, 0)
    })
  })

  // -------------------------------------------------------------------------
  // cancelPlacement
  // -------------------------------------------------------------------------
  describe('cancelPlacement', () => {
    it('calls bridge with "cancelPlacement"', async () => {
      const bridge = createMockBridge()
      await cancelPlacement(bridge)

      expect(bridge).toHaveBeenCalledWith('cancelPlacement')
    })
  })

  // -------------------------------------------------------------------------
  // placeSymbolInstance
  // -------------------------------------------------------------------------
  describe('placeSymbolInstance', () => {
    it('calls bridge with all option fields and defaults', async () => {
      const placementResult = { success: true, uuid: 'def-456' }
      const bridge = createMockBridge(placementResult)

      const result = await placeSymbolInstance(bridge, 'Oak', { x: 50, y: 75 }, {
        scale: 2.0,
        rotation: 90,
        radiusPts: 36,
        color: '#FF0000',
        opacity: 80,
        circleFillStyle: 'outline',
        skipBoundsCheck: true,
      })

      expect(bridge).toHaveBeenCalledWith(
        'placeSymbolInstance',
        'Oak',
        50,
        75,
        2.0,
        90,
        36,
        '#FF0000',
        undefined, // refDesignator
        undefined, // letter
        undefined, // displayLabel
        80,
        undefined, // fontName
        undefined, // fontSize
        true,
        'outline',
      )
      expect(result).toEqual(placementResult)
    })

    it('applies default values when options are omitted', async () => {
      const bridge = createMockBridge({ success: true, uuid: 'ghi-789' })
      await placeSymbolInstance(bridge, 'Elm', { x: 10, y: 20 })

      expect(bridge).toHaveBeenCalledWith(
        'placeSymbolInstance',
        'Elm',
        10,
        20,
        1.0,        // default scale
        0,          // default rotation
        undefined,  // radiusPts
        undefined,  // color
        undefined,  // refDesignator
        undefined,  // letter
        undefined,  // displayLabel
        undefined,  // opacity
        undefined,  // fontName
        undefined,  // fontSize
        undefined,  // skipBoundsCheck
        'filled',   // default circleFillStyle
      )
    })
  })

  // -------------------------------------------------------------------------
  // setItemMetadata
  // -------------------------------------------------------------------------
  describe('setItemMetadata', () => {
    it('serializes notes to JSON and calls bridge', async () => {
      const bridge = createMockBridge()
      const notes = { species: 'Quercus alba', dbh: 12 }
      await setItemMetadata(bridge, 'uuid-001', 'White Oak', notes)

      expect(bridge).toHaveBeenCalledWith(
        'setItemMetadata',
        'uuid-001',
        'White Oak',
        JSON.stringify(notes),
      )
    })

    it('handles empty notes object', async () => {
      const bridge = createMockBridge()
      await setItemMetadata(bridge, 'uuid-002', 'Empty', {})

      expect(bridge).toHaveBeenCalledWith('setItemMetadata', 'uuid-002', 'Empty', '{}')
    })
  })

  // -------------------------------------------------------------------------
  // importAsset
  // -------------------------------------------------------------------------
  describe('importAsset', () => {
    it('passes filePath and options to bridge', async () => {
      const result = { success: true, uuid: 'imp-001' }
      const bridge = createMockBridge(result)
      const opts = { position: { x: 100, y: 200 }, width: 300 }

      const response = await importAsset(bridge, '/path/to/asset.svg', opts)

      expect(bridge).toHaveBeenCalledWith('importAsset', '/path/to/asset.svg', opts)
      expect(response).toEqual(result)
    })

    it('passes empty options by default', async () => {
      const bridge = createMockBridge({ success: true, uuid: 'imp-002' })
      await importAsset(bridge, '/file.ai')

      expect(bridge).toHaveBeenCalledWith('importAsset', '/file.ai', {})
    })
  })
})
