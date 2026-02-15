import type { Script } from '../types'
import { QueryTextFrames, GetTextContent } from '@/sdk/generated/customRoutes'

const listTextFrames: Script = {
  id: 'list-text-frames',
  name: 'List Text Frames',
  description: 'Find all text frames and show their content',
  category: 'text',
  async run() {
    const result = await QueryTextFrames()
    const frames = result.frames as unknown as Array<{ handle: number; name: string }>

    if (!frames || frames.length === 0) {
      return {
        success: true,
        message: 'No text frames found',
        data: [],
      }
    }

    const details = []
    for (const frame of frames) {
      let contents = ''
      try {
        const tc = await GetTextContent(String(frame.handle))
        contents = tc.contents
      } catch {
        contents = '(unable to read)'
      }
      details.push({
        handle: frame.handle,
        name: frame.name,
        preview: contents.length > 80 ? contents.slice(0, 80) + '...' : contents,
      })
    }

    return {
      success: true,
      message: `${frames.length} text frame(s)`,
      data: details,
    }
  },
}

export default listTextFrames
