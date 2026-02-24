import type { Script } from '../types'
import { getDocumentInfo, getUnits } from '@nuxp/sdk'
import { getBridgeInstance } from '@nuxp/sdk/generated/_bridge'

const sdkDocumentAdapter: Script = {
  id: 'sdk-document-adapter',
  name: 'DocumentAdapter',
  description: 'Higher-level document queries using the BridgeCallFn dependency injection pattern (requires plugin)',
  category: 'sdk',
  async run() {
    // The DocumentAdapter uses BridgeCallFn — a pure function signature
    // instead of importing a global singleton. This makes it testable
    // and decoupled from any specific Bridge instance.
    const bridge = getBridgeInstance()
    const call = bridge.call.bind(bridge)

    const [docInfo, units] = await Promise.all([
      getDocumentInfo(call),
      getUnits(call),
    ])

    return {
      success: true,
      message: `Document: ${docInfo.name} (${units.unitsName})`,
      data: {
        documentInfo: {
          name: docInfo.name,
          fullName: docInfo.fullName,
          path: docInfo.path,
          saved: docInfo.saved,
          dimensions: `${docInfo.width}" x ${docInfo.height}" (${docInfo.widthPoints}pt x ${docInfo.heightPoints}pt)`,
          artboards: `${docInfo.artboardCount} (active: #${docInfo.currentArtboard})`,
        },
        units,
        pattern: 'DocumentAdapter functions accept BridgeCallFn as first param — no global state needed',
      },
    }
  },
}

export default sdkDocumentAdapter
