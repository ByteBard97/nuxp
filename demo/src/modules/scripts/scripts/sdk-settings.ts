import type { Script } from '../types'
import { SettingsService } from '@nuxp/sdk'

interface DemoSettings extends Record<string, unknown> {
  theme: string
  zoom: number
  sidebarVisible: boolean
  lastActiveTab: string
}

const sdkSettings: Script = {
  id: 'sdk-settings',
  name: 'SettingsService',
  description: 'Type-safe localStorage persistence with defaults, get/set, and reset',
  category: 'sdk',
  async run() {
    const defaults: Partial<DemoSettings> = {
      theme: 'dark',
      zoom: 1,
      sidebarVisible: true,
      lastActiveTab: 'scripts',
    }

    const settings = new SettingsService<DemoSettings>('nuxp-demo-sdk-test', defaults)

    // Reset to start clean
    settings.reset()

    // Read defaults
    const step1 = settings.getAll()

    // Set individual values
    settings.set('theme', 'light')
    settings.set('zoom', 1.5)
    settings.set('lastActiveTab', 'tests')

    // Read back
    const step2 = {
      theme: settings.get('theme'),
      zoom: settings.get('zoom'),
      sidebarVisible: settings.get('sidebarVisible'), // still default
      lastActiveTab: settings.get('lastActiveTab'),
    }

    // Batch save
    settings.save({ sidebarVisible: false, zoom: 2 })
    const step3 = settings.getAll()

    // Reset and verify defaults return
    settings.reset()
    const step4 = settings.getAll()

    return {
      success: true,
      message: 'SettingsService lifecycle: defaults \u2192 set \u2192 save \u2192 reset',
      data: {
        'step1_defaults': step1,
        'step2_individual_sets': step2,
        'step3_after_batch_save': step3,
        'step4_after_reset': step4,
      },
    }
  },
}

export default sdkSettings
