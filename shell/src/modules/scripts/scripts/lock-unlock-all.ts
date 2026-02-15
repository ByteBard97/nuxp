import type { Script } from '../types'
import { CountLayers, GetNthLayer, SetLayerEditable } from '@/sdk/generated/AILayerSuite'

const lockUnlockAll: Script = {
  id: 'lock-unlock-all',
  name: 'Lock/Unlock All Layers',
  description: 'Lock or unlock all layers at once',
  category: 'layers',
  params: [
    { name: 'lock', label: 'Lock', type: 'boolean', default: false },
  ],
  async run(params) {
    const lock = params.lock as boolean
    const count = await CountLayers()

    for (let i = 0; i < count; i++) {
      const handle = await GetNthLayer(i)
      await SetLayerEditable(handle, !lock)
    }

    return {
      success: true,
      message: `${lock ? 'Locked' : 'Unlocked'} ${count} layer(s)`,
    }
  },
}

export default lockUnlockAll
