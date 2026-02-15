import type { Script } from '../types'
import { GetSelection } from '@/sdk/generated/customRoutes'
import { DuplicateArt } from '@/sdk/generated/AIArtSuite'

// Paint order constants from AITypes.h
const kPlaceAbove = 3
const kPlaceDefault = 0

const duplicateSelected: Script = {
  id: 'duplicate-selected',
  name: 'Duplicate Selected',
  description: 'Duplicate all selected art objects',
  category: 'objects',
  async run() {
    const sel = await GetSelection()

    if (sel.count === 0) {
      return { success: false, message: 'Nothing selected' }
    }

    const newHandles: number[] = []
    for (const handle of sel.handles) {
      const newHandle = await DuplicateArt(handle, kPlaceAbove, kPlaceDefault)
      newHandles.push(newHandle)
    }

    return {
      success: true,
      message: `Duplicated ${sel.count} object(s)`,
      data: { originalCount: sel.count, newHandles },
    }
  },
}

export default duplicateSelected
