import type { Script } from '../types'
import { CreateTextFrame } from '@/sdk/generated/customRoutes'

const createTextFrame: Script = {
  id: 'create-text-frame',
  name: 'Create Text Frame',
  description: 'Create a new point text frame with custom text',
  category: 'text',
  params: [
    { name: 'text', label: 'Text', type: 'string', default: 'Hello NUXP' },
    { name: 'x', label: 'X', type: 'number', default: 100 },
    { name: 'y', label: 'Y', type: 'number', default: -200 },
  ],
  async run(params) {
    const result = await CreateTextFrame({
      x: params.x as number,
      y: params.y as number,
      contents: params.text as string,
    })

    return {
      success: result.success,
      message: result.success
        ? `Created text frame (handle ${result.artId})`
        : 'Failed to create text frame',
      data: result,
    }
  },
}

export default createTextFrame
