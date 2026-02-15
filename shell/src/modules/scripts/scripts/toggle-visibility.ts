import type { Script } from '../types'
import { CountLayers, GetNthLayer, SetLayerVisible } from '@/sdk/generated/AILayerSuite'

const toggleVisibility: Script = {
  id: 'toggle-visibility',
  name: 'Toggle All Visibility',
  description: 'Show or hide all layers at once',
  category: 'layers',
  inspiredBy: 'Layer visibility scripts (sky-chaser)',
  params: [
    { name: 'visible', label: 'Visible', type: 'boolean', default: true },
  ],
  async run(params) {
    const visible = params.visible as boolean
    const count = await CountLayers()

    for (let i = 0; i < count; i++) {
      const handle = await GetNthLayer(i)
      await SetLayerVisible(handle, visible)
    }

    return {
      success: true,
      message: `Set ${count} layer(s) to ${visible ? 'visible' : 'hidden'}`,
    }
  },
}

export default toggleVisibility
