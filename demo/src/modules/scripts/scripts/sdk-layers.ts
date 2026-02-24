import type { Script } from '../types'
import { getAllLayers, getLayerByName, getLayerArt } from '@nuxp/sdk'
import { getBridgeInstance } from '@nuxp/sdk/generated/_bridge'

const sdkLayers: Script = {
  id: 'sdk-layers',
  name: 'Layer Primitives',
  description: 'List layers, look up by name, and count art per layer (requires plugin)',
  category: 'sdk',
  async run() {
    const bridge = getBridgeInstance()
    const call = bridge.call.bind(bridge)

    // List all layers in the document
    const layers = await getAllLayers(call)

    // For each layer, count art objects on it
    const layerDetails = await Promise.all(
      layers.map(async (layer) => {
        const art = await getLayerArt(call, layer.handle)
        return {
          name: layer.name,
          handle: layer.handle,
          artCount: art.length,
        }
      })
    )

    // Try to look up the first layer by name as a demo of getLayerByName
    let lookupResult: { searched: string; found: boolean; handle: number | null } | null = null
    if (layers.length > 0) {
      const firstName = layers[0].name
      const handle = await getLayerByName(call, firstName)
      lookupResult = {
        searched: firstName,
        found: handle !== null,
        handle,
      }
    }

    return {
      success: true,
      message: `Document has ${layers.length} layer(s)`,
      data: {
        layers: layerDetails,
        layerLookup: lookupResult,
        availableFunctions: [
          'getAllLayers(call) — list all layers with handles and names',
          'getLayerByName(call, name) — find a layer by its title',
          'getLayerArt(call, layerHandle) — get all art handles on a layer',
          'createLayer(call, name, position?) — create a new named layer',
        ],
      },
    }
  },
}

export default sdkLayers
