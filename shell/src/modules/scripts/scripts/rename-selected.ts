import type { Script } from '../types'
import { GetSelection } from '@/sdk/generated/customRoutes'
import { SetArtName } from '@/sdk/generated/AIArtSuite'

const renameSelected: Script = {
  id: 'rename-selected',
  name: 'Rename Selected',
  description: 'Set name on selected items with a prefix + index',
  category: 'objects',
  inspiredBy: 'RenameItems.jsx (creold)',
  params: [
    { name: 'prefix', label: 'Prefix', type: 'string', default: 'NUXP_' },
  ],
  async run(params) {
    const prefix = params.prefix as string
    const sel = await GetSelection()

    if (sel.count === 0) {
      return { success: false, message: 'Nothing selected' }
    }

    for (let i = 0; i < sel.handles.length; i++) {
      await SetArtName(sel.handles[i], `${prefix}${i + 1}`)
    }

    return {
      success: true,
      message: `Renamed ${sel.count} object(s) with prefix "${prefix}"`,
    }
  },
}

export default renameSelected
