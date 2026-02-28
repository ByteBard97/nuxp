import type { Script } from '../types'
import { createGroup, getGroupChildren, ArtType } from '@nuxp/sdk'
import { getBridgeInstance } from '@nuxp/sdk/generated/_bridge'
import { GetArtType } from '@nuxp/sdk/generated/AIArtSuite'

const sdkGroups: Script = {
  id: 'sdk-groups',
  name: 'Group Primitives',
  description: 'Create groups and inspect their children using group primitives (requires plugin)',
  category: 'sdk',
  async run() {
    const bridge = getBridgeInstance()
    const call = bridge.call.bind(bridge)

    // Create a new empty group in the document
    const groupHandle = await createGroup(call)

    // Query the children of the newly created (empty) group
    const children = await getGroupChildren(call, groupHandle)

    // Get the art type of the group to confirm it was created correctly
    const artType = await GetArtType(groupHandle)

    return {
      success: true,
      message: `Created group (handle ${groupHandle}) with ${children.length} children`,
      data: {
        groupHandle,
        artType,
        artTypeName: artType === ArtType.kGroupArt ? 'kGroupArt' : `unknown (${artType})`,
        childCount: children.length,
        children,
        availableFunctions: [
          'createGroup(call, paintOrder?, prep?) — create an empty group',
          'addToGroup(call, groupHandle, artHandle) — move art into a group',
          'ungroup(call, groupHandle) — dissolve group, returning freed children',
          'getGroupChildren(call, groupHandle) — list direct child handles',
        ],
        note: 'Use addToGroup() to move existing art objects into this group, or ungroup() to dissolve it',
      },
    }
  },
}

export default sdkGroups
