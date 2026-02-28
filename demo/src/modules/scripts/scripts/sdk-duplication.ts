import type { Script } from '../types'
import { getAllLayers, getLayerArt, duplicateArt } from '@nuxp/sdk'
import { getBridgeInstance } from '@nuxp/sdk/generated/_bridge'
import { GetArtType } from '@nuxp/sdk/generated/AIArtSuite'

const sdkDuplication: Script = {
  id: 'sdk-duplication',
  name: 'Duplication Primitives',
  description: 'Duplicate art objects on the current layer (requires plugin, document with art)',
  category: 'sdk',
  async run() {
    const bridge = getBridgeInstance()
    const call = bridge.call.bind(bridge)

    // Find the first art object in the document to duplicate
    const layers = await getAllLayers(call)
    if (layers.length === 0) {
      return {
        success: false,
        message: 'No layers found — open a document with at least one layer',
      }
    }

    // Walk layers until we find one with art
    let sourceHandle: number | null = null
    let sourceLayerName = ''
    for (const layer of layers) {
      const art = await getLayerArt(call, layer.handle)
      if (art.length > 0) {
        sourceHandle = art[0]
        sourceLayerName = layer.name
        break
      }
    }

    if (sourceHandle === null) {
      return {
        success: false,
        message: 'No art objects found in any layer — add some art to the document first',
      }
    }

    // Get art type of the source for display
    const sourceType = await GetArtType(sourceHandle)

    // Duplicate the art object (places above the original by default)
    const duplicateHandle = await duplicateArt(call, sourceHandle)
    const duplicateType = await GetArtType(duplicateHandle)

    return {
      success: true,
      message: `Duplicated art from layer "${sourceLayerName}" (handle ${sourceHandle} -> ${duplicateHandle})`,
      data: {
        source: {
          handle: sourceHandle,
          artType: sourceType,
          layer: sourceLayerName,
        },
        duplicate: {
          handle: duplicateHandle,
          artType: duplicateType,
        },
        availableFunctions: [
          'duplicateArt(call, artHandle, paintOrder?, prep?) — duplicate art at a paint-order position',
          'duplicateArtToPosition(call, artHandle, targetHandle, paintOrder) — duplicate and reorder relative to a target',
        ],
        note: 'The duplicate is placed directly above the original in the paint order',
      },
    }
  },
}

export default sdkDuplication
