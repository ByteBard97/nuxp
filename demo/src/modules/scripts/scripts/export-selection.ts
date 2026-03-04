import type { Script } from '../types'
import { ExportSelectionAsSvg } from '@nuxp/sdk/generated/customRoutes'

const SVG_PREVIEW_LENGTH = 200

const exportSelection: Script = {
  id: 'export-selection',
  name: 'Export Selection as SVG',
  description: 'Export only the currently selected art objects as SVG',
  category: 'selection',
  async run() {
    const result = await ExportSelectionAsSvg()

    if (!result.success) {
      return {
        success: false,
        message: result.message || 'Selection export failed',
        data: result,
      }
    }

    const svgPreview = result.svgContent
      ? result.svgContent.substring(0, SVG_PREVIEW_LENGTH) + '...'
      : '(empty)'

    return {
      success: true,
      message: result.message,
      data: {
        filePath: result.filePath,
        svgByteLength: result.svgContent?.length ?? 0,
        svgPreview,
        artBounds: result.artBounds,
      },
    }
  },
}

export default exportSelection
