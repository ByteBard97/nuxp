import type { Script } from '../types'
import { ExportSvgViaAction } from '@nuxp/sdk/generated/customRoutes'

const SVG_PREVIEW_LENGTH = 200

const sdkExport: Script = {
  id: 'sdk-export',
  name: 'SVG Export',
  description: 'Export the current document as SVG using the ActionManager approach',
  category: 'sdk',
  async run() {
    const result = await ExportSvgViaAction({})

    if (!result.success) {
      return {
        success: false,
        message: result.message || 'SVG export failed',
        data: result,
      }
    }

    const svgPreview = result.svgContent
      ? result.svgContent.substring(0, SVG_PREVIEW_LENGTH) + '...'
      : '(written to file, not read back)'

    return {
      success: true,
      message: `SVG exported to ${result.filePath}`,
      data: {
        filePath: result.filePath,
        svgByteLength: result.svgContent?.length ?? 0,
        svgPreview,
        availableFunctions: [
          'ExportSvgViaAction({ outputPath? }) — export via AIActionManagerSuite::PlayActionEvent',
          'ExportSvgViaFileFormat({ outputPath? }) — export via AIFileFormatSuite::GoExport',
          'ExportSelectionAsSvg() — export only the current selection as SVG',
        ],
        note: 'Omit outputPath to get SVG content returned directly; provide a path to write to disk only',
      },
    }
  },
}

export default sdkExport
