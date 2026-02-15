import type { Script } from '../types'
import { GetDocumentInfo } from '@/sdk/generated/customRoutes'

const documentInfo: Script = {
  id: 'document-info',
  name: 'Document Info',
  description: 'Show document name, path, saved state, and artboard dimensions',
  category: 'document',
  inspiredBy: 'ObjectsCounter.jsx (creold)',
  async run() {
    const info = await GetDocumentInfo()
    return {
      success: true,
      message: `Document: ${info.name}`,
      data: info,
    }
  },
}

export default documentInfo
