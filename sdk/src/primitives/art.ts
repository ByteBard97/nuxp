/**
 * Generic art object primitives for the Illustrator bridge.
 *
 * Each function accepts a `bridge` callable as its first argument, making these
 * operations independent of any specific bridge implementation. Consumers pass
 * in their own bridge function (e.g. from HostBridge.callHost).
 *
 * These are the permanent bridge primitives -- the C++ side never needs to change.
 */

import type { ArtBounds, ArtChild, ArtboardInfo, BridgeCallFn } from './types'

/**
 * Get the bounding rectangle of an art object.
 *
 * @param bridge - Bridge callable for host communication
 * @param uuid - UUID of the art object
 * @returns The art bounds along with the UUID
 */
export async function getArtBounds(
  bridge: BridgeCallFn,
  uuid: string
): Promise<ArtBounds & { uuid: string }> {
  return bridge('getArtBounds', uuid)
}

/**
 * Apply a translation and/or scale transform to an art object.
 *
 * @param bridge - Bridge callable for host communication
 * @param uuid - UUID of the art object
 * @param transform - Transform parameters (tx/ty for translation, sx/sy for scale)
 * @returns Success status and UUID
 */
export async function transformArt(
  bridge: BridgeCallFn,
  uuid: string,
  transform: { tx?: number; ty?: number; sx?: number; sy?: number }
): Promise<{ success: boolean; uuid: string }> {
  return bridge('transformArt', uuid, transform)
}

/**
 * Show or hide an art object.
 *
 * @param bridge - Bridge callable for host communication
 * @param uuid - UUID of the art object
 * @param visible - Whether the art should be visible
 * @returns Success status, UUID, and new visibility state
 */
export async function setArtVisibility(
  bridge: BridgeCallFn,
  uuid: string,
  visible: boolean
): Promise<{ success: boolean; uuid: string; visible: boolean }> {
  return bridge('setArtVisibility', uuid, { visible })
}

/**
 * List direct children of a group art object.
 *
 * @param bridge - Bridge callable for host communication
 * @param uuid - UUID of the group art object
 * @returns The group UUID and its children
 */
export async function getArtChildren(
  bridge: BridgeCallFn,
  uuid: string
): Promise<{ uuid: string; children: ArtChild[] }> {
  return bridge('getArtChildren', uuid)
}

/**
 * Get the first child of a group that matches a specific art type.
 *
 * @param bridge - Bridge callable for host communication
 * @param uuid - UUID of the group art object
 * @param artType - Numeric art type constant (see ArtType)
 * @returns The matching child's info, or null if not found
 */
export async function getGroupChildByType(
  bridge: BridgeCallFn,
  uuid: string,
  artType: number
): Promise<{ uuid: string; name: string; artType: number; bounds: ArtBounds } | null> {
  const result = await bridge('getGroupChildByType', uuid, { artType })
  if (result?.error) return null
  return result
}

/**
 * Rename an art object.
 *
 * @param bridge - Bridge callable for host communication
 * @param uuid - UUID of the art object
 * @param name - New name for the art object
 * @returns Success status, UUID, and new name
 */
export async function setArtName(
  bridge: BridgeCallFn,
  uuid: string,
  name: string
): Promise<{ success: boolean; uuid: string; name: string }> {
  return bridge('setArtName', uuid, { name })
}

/**
 * Delete an art object (no cascade).
 *
 * @param bridge - Bridge callable for host communication
 * @param uuid - UUID of the art object to delete
 * @returns Success status and UUID
 */
export async function deleteArt(
  bridge: BridgeCallFn,
  uuid: string
): Promise<{ success: boolean; uuid: string }> {
  return bridge('deleteArt', uuid)
}

/**
 * Find art objects by name substring.
 *
 * @param bridge - Bridge callable for host communication
 * @param name - Name or substring to search for
 * @returns Matching art objects
 */
export async function findArtByName(
  bridge: BridgeCallFn,
  name: string
): Promise<Array<{ uuid: string; name: string; artType: number }>> {
  return bridge('findArtByName', { name })
}

/**
 * Get artboard dimensions and position.
 *
 * @param bridge - Bridge callable for host communication
 * @returns Artboard info including width, height, and edge coordinates
 */
export async function getArtboardInfo(
  bridge: BridgeCallFn
): Promise<ArtboardInfo> {
  return bridge('getArtboardInfo')
}
