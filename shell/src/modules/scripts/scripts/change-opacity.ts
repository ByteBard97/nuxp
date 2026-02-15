import type { Script } from '../types'
import { GetSelection } from '@/sdk/generated/customRoutes'
import { SetOpacity } from '@/sdk/generated/AIBlendStyleSuite'

const changeOpacity: Script = {
  id: 'change-opacity',
  name: 'Change Opacity',
  description: 'Set opacity on selected items (0-100%)',
  category: 'objects',
  inspiredBy: 'Opacity scripts (sky-chaser)',
  params: [
    { name: 'opacity', label: 'Opacity %', type: 'number', default: 50, min: 0, max: 100, step: 5 },
  ],
  async run(params) {
    const opacityPct = params.opacity as number
    const sel = await GetSelection()

    if (sel.count === 0) {
      return { success: false, message: 'Nothing selected' }
    }

    // SDK expects 0.0-1.0
    const opacityValue = opacityPct / 100

    for (const handle of sel.handles) {
      await SetOpacity(handle, opacityValue)
    }

    return {
      success: true,
      message: `Set opacity to ${opacityPct}% on ${sel.count} object(s)`,
    }
  },
}

export default changeOpacity
