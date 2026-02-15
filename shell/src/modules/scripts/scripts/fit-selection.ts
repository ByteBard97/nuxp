import type { Script } from '../types'
import { FitSelectionInView } from '@/sdk/generated/customRoutes'

const fitSelection: Script = {
  id: 'fit-selection',
  name: 'Fit Selection in View',
  description: 'Zoom to fit the current selection',
  category: 'view',
  async run() {
    const result = await FitSelectionInView()
    return {
      success: result.success,
      message: result.success ? 'Zoomed to fit selection' : 'Failed to fit selection (nothing selected?)',
    }
  },
}

export default fitSelection
