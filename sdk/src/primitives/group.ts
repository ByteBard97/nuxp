/**
 * Group art primitives for the Illustrator bridge.
 *
 * Each function accepts a `bridge` callable as its first argument for API
 * consistency with other primitives. Internally, these compose the
 * auto-generated AIArtSuite functions to provide higher-level group operations.
 */

import type { BridgeCallFn } from './types'
import { ArtType } from './types'
import {
  NewArt,
  ReorderArt,
  DisposeArt,
  GetArtFirstChild,
  GetArtSibling,
} from '../generated/AIArtSuite'

/** Paint order constants matching AITypes.h */
const kPlaceInsideOnTop = 4

/**
 * Create a new empty group art object.
 *
 * @param _bridge - Bridge callable (unused; included for API consistency)
 * @param paintOrder - Paint order position (default: 0 = kPlaceAbove)
 * @param prep - Prepositional art handle for placement (default: 0)
 * @returns Handle to the newly created group art
 */
export async function createGroup(
  _bridge: BridgeCallFn,
  paintOrder: number = 0,
  prep: number = 0
): Promise<number> {
  return NewArt(ArtType.kGroupArt, paintOrder, prep)
}

/**
 * Move an art object inside a group (as the topmost child).
 *
 * @param _bridge - Bridge callable (unused; included for API consistency)
 * @param groupHandle - Handle to the target group
 * @param artHandle - Handle to the art object to move into the group
 */
export async function addToGroup(
  _bridge: BridgeCallFn,
  groupHandle: number,
  artHandle: number
): Promise<void> {
  await ReorderArt(artHandle, kPlaceInsideOnTop, groupHandle)
}

/**
 * Ungroup a group art object by moving all children out, then disposing the
 * empty group shell.
 *
 * Children are reordered to sit directly before the group in paint order
 * (kPlaceAbove = 0), preserving their relative stacking.
 *
 * @param _bridge - Bridge callable (unused; included for API consistency)
 * @param groupHandle - Handle to the group to ungroup
 * @returns Array of handles to the formerly-grouped children
 */
export async function ungroup(
  _bridge: BridgeCallFn,
  groupHandle: number
): Promise<number[]> {
  const children = await getGroupChildren(_bridge, groupHandle)

  // Move each child above the group (paint order 0 = kPlaceAbove)
  for (const child of children) {
    await ReorderArt(child, 0, groupHandle)
  }

  await DisposeArt(groupHandle)
  return children
}

/**
 * Collect all direct child art handles of a group.
 *
 * Walks the first-child / next-sibling linked list exposed by the SDK.
 * A handle value of 0 signals the end of the list.
 *
 * @param _bridge - Bridge callable (unused; included for API consistency)
 * @param groupHandle - Handle to the group art object
 * @returns Array of child art handles in paint-order
 */
export async function getGroupChildren(
  _bridge: BridgeCallFn,
  groupHandle: number
): Promise<number[]> {
  const children: number[] = []
  let current = await GetArtFirstChild(groupHandle)

  while (current !== 0) {
    children.push(current)
    current = await GetArtSibling(current)
  }

  return children
}
