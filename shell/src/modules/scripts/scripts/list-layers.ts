import type { Script } from '../types'
import { CountLayers, GetNthLayer, GetLayerTitle, GetLayerVisible, GetLayerEditable } from '@/sdk/generated/AILayerSuite'

const listLayers: Script = {
  id: 'list-layers',
  name: 'List Layers',
  description: 'Show all layers with visibility and lock state',
  category: 'layers',
  async run() {
    const count = await CountLayers()
    const layers = []

    for (let i = 0; i < count; i++) {
      const handle = await GetNthLayer(i)
      const title = await GetLayerTitle(handle)
      const visible = await GetLayerVisible(handle)
      const editable = await GetLayerEditable(handle)
      layers.push({
        index: i,
        title,
        visible,
        locked: !editable,
      })
    }

    return {
      success: true,
      message: `${count} layer(s)`,
      data: layers,
    }
  },
}

export default listLayers
