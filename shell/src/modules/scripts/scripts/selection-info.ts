import type { Script } from '../types'
import { GetSelection } from '@/sdk/generated/customRoutes'
import { GetArtType } from '@/sdk/generated/AIArtSuite'

const ART_TYPE_NAMES: Record<number, string> = {
  0: 'Unknown',
  1: 'Group',
  2: 'Path',
  3: 'Compound Path',
  7: 'Placed',
  9: 'Raster',
  10: 'Plugin',
  11: 'Mesh',
  12: 'Text Frame',
  13: 'Symbol',
}

const selectionInfo: Script = {
  id: 'selection-info',
  name: 'Selection Info',
  description: 'Show what\'s selected: count and types',
  category: 'selection',
  async run() {
    const sel = await GetSelection()

    if (sel.count === 0) {
      return {
        success: true,
        message: 'Nothing selected',
        data: { count: 0, items: [] },
      }
    }

    const typeCounts: Record<string, number> = {}
    for (const handle of sel.handles) {
      try {
        const typeNum = await GetArtType(handle)
        const typeName = ART_TYPE_NAMES[typeNum] ?? `Type ${typeNum}`
        typeCounts[typeName] = (typeCounts[typeName] ?? 0) + 1
      } catch {
        typeCounts['Unknown'] = (typeCounts['Unknown'] ?? 0) + 1
      }
    }

    return {
      success: true,
      message: `${sel.count} object(s) selected`,
      data: { count: sel.count, types: typeCounts },
    }
  },
}

export default selectionInfo
