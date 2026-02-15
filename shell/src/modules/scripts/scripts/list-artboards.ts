import type { Script } from '../types'
import { GetDocumentInfo } from '@/sdk/generated/customRoutes'

const listArtboards: Script = {
  id: 'list-artboards',
  name: 'List Artboards',
  description: 'Show all artboards with dimensions',
  category: 'document',
  inspiredBy: 'ArtboardsFinder.jsx (creold)',
  async run() {
    const info = await GetDocumentInfo()
    const artboards = info.artboards as unknown as Array<Record<string, unknown>>

    return {
      success: true,
      message: `${artboards?.length ?? 0} artboard(s)`,
      data: artboards,
    }
  },
}

export default listArtboards
