import type { Script } from '../types'
import { QueryPathItems, QueryTextFrames, QueryLayers } from '@/sdk/generated/customRoutes'

const objectCounter: Script = {
  id: 'object-counter',
  name: 'Object Counter',
  description: 'Count objects by type: paths, text frames, layers',
  category: 'document',
  inspiredBy: 'ObjectsCounter.jsx (creold)',
  async run() {
    const [paths, textFrames, layers] = await Promise.all([
      QueryPathItems(),
      QueryTextFrames(),
      QueryLayers(),
    ])

    const counts = {
      pathItems: paths.count,
      textFrames: textFrames.count,
      layers: layers.count,
    }
    const total = counts.pathItems + counts.textFrames

    return {
      success: true,
      message: `${total} objects across ${counts.layers} layers`,
      data: counts,
    }
  },
}

export default objectCounter
