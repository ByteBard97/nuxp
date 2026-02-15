import type { Script } from '../types'
import { FitArtboardInView } from '@/sdk/generated/customRoutes'

const fitArtboard: Script = {
  id: 'fit-artboard',
  name: 'Fit Artboard in View',
  description: 'Zoom to fit the active artboard',
  category: 'view',
  async run() {
    const result = await FitArtboardInView()
    return {
      success: result.success,
      message: result.success ? 'Zoomed to fit artboard' : 'Failed to fit artboard',
    }
  },
}

export default fitArtboard
