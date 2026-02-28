/**
 * Art duplication primitives for the Illustrator bridge.
 *
 * Each function accepts a `bridge` callable as its first argument for API
 * consistency with other primitives. Internally, these compose the
 * auto-generated AIArtSuite functions.
 */

import type { BridgeCallFn } from './types'
import { DuplicateArt, ReorderArt } from '../generated/AIArtSuite'

/**
 * Duplicate an art object at the given paint-order position.
 *
 * Thin wrapper around `AIArtSuite.DuplicateArt` with sensible defaults.
 *
 * @param _bridge - Bridge callable (unused; included for API consistency)
 * @param artHandle - Handle to the art object to duplicate
 * @param paintOrder - Paint order position for the copy (default: 0 = kPlaceAbove)
 * @param prep - Prepositional art handle for placement (default: 0)
 * @returns Handle to the newly created duplicate art
 */
export async function duplicateArt(
  _bridge: BridgeCallFn,
  artHandle: number,
  paintOrder: number = 0,
  prep: number = 0
): Promise<number> {
  return DuplicateArt(artHandle, paintOrder, prep)
}

/**
 * Duplicate an art object and reorder the copy to a specific position
 * relative to a target art object.
 *
 * First creates the duplicate, then moves it to the desired location
 * in the art tree.
 *
 * @param _bridge - Bridge callable (unused; included for API consistency)
 * @param artHandle - Handle to the art object to duplicate
 * @param targetHandle - Handle to the art object to position relative to
 * @param paintOrder - Paint order relative to targetHandle (e.g. 0 = kPlaceAbove)
 * @returns Handle to the newly created and repositioned duplicate art
 */
export async function duplicateArtToPosition(
  _bridge: BridgeCallFn,
  artHandle: number,
  targetHandle: number,
  paintOrder: number
): Promise<number> {
  // Duplicate in place first (above the original)
  const newArt = await DuplicateArt(artHandle, 0, 0)
  // Then reorder to the desired position relative to the target
  await ReorderArt(newArt, paintOrder, targetHandle)
  return newArt
}
