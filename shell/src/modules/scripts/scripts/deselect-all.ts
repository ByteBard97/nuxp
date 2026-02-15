import type { Script } from '../types'
import { DeselectAll } from '@/sdk/generated/customRoutes'

const deselectAll: Script = {
  id: 'deselect-all',
  name: 'Deselect All',
  description: 'Clear the current selection',
  category: 'selection',
  async run() {
    const result = await DeselectAll()
    return {
      success: result.success,
      message: result.success ? 'Selection cleared' : 'Failed to deselect',
    }
  },
}

export default deselectAll
