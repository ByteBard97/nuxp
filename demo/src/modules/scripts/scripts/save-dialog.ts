import type { Script } from '../types'
import { ShowFileSaveDialog } from '@nuxp/sdk/generated/customRoutes'

const saveDialog: Script = {
  id: 'save-dialog',
  name: 'File Save Dialog',
  description: 'Show a native file save dialog and return the selected path',
  category: 'document',
  params: [
    { name: 'defaultName', label: 'Default Filename', type: 'string', default: 'export.svg' },
    { name: 'title', label: 'Dialog Title', type: 'string', default: 'Save File' },
  ],
  async run(params) {
    const result = await ShowFileSaveDialog({
      defaultName: params.defaultName as string,
      title: params.title as string,
      fileTypes: ['svg', 'ai', 'pdf'],
    })

    if (result.cancelled) {
      return {
        success: true,
        message: 'User cancelled the dialog',
        data: { cancelled: true },
      }
    }

    return {
      success: true,
      message: `Selected: ${result.path}`,
      data: {
        path: result.path,
        cancelled: false,
      },
    }
  },
}

export default saveDialog
