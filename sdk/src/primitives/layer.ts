/**
 * Layer primitives for the Illustrator bridge.
 *
 * Each function accepts a `bridge` callable as its first argument for API
 * consistency with other primitives. Internally, these compose the
 * auto-generated AILayerSuite and AIArtSuite functions.
 */

import type { BridgeCallFn } from './types'
import {
  CountLayers,
  GetNthLayer,
  GetLayerTitle,
  InsertLayer,
  SetLayerTitle,
  GetLayerByTitle,
} from '../generated/AILayerSuite'
import {
  GetFirstArtOfLayer,
  GetArtSibling,
} from '../generated/AIArtSuite'

/** Info returned for each layer in a listing. */
export interface LayerInfo {
  /** SDK layer handle */
  handle: number
  /** Display name of the layer */
  name: string
}

/**
 * Find a layer by its display title.
 *
 * Uses the SDK's native `GetLayerByTitle` for an efficient single lookup.
 * Returns `null` if no layer with the given title exists (handle is 0).
 *
 * @param _bridge - Bridge callable (unused; included for API consistency)
 * @param name - Exact layer title to search for
 * @returns Layer handle, or `null` if not found
 */
export async function getLayerByName(
  _bridge: BridgeCallFn,
  name: string
): Promise<number | null> {
  try {
    const handle = await GetLayerByTitle(name)
    return handle !== 0 ? handle : null
  } catch {
    // GetLayerByTitle throws when the layer does not exist
    return null
  }
}

/**
 * Create a new layer with the given display title.
 *
 * @param _bridge - Bridge callable (unused; included for API consistency)
 * @param name - Display title for the new layer
 * @param position - Paint order position (default: 0 = kPlaceAbove)
 * @returns Handle to the newly created layer
 */
export async function createLayer(
  _bridge: BridgeCallFn,
  name: string,
  position: number = 0
): Promise<number> {
  // InsertLayer takes a reference layer and paint order.
  // Passing 0 as the reference layer creates at the default position.
  const newLayer = await InsertLayer(0, position)
  await SetLayerTitle(newLayer, name)
  return newLayer
}

/**
 * Collect all art handles on a given layer.
 *
 * Walks the first-art / next-sibling linked list for the layer.
 * A handle value of 0 signals the end of the list.
 *
 * @param _bridge - Bridge callable (unused; included for API consistency)
 * @param layerHandle - Handle to the layer
 * @returns Array of art handles on the layer in paint-order
 */
export async function getLayerArt(
  _bridge: BridgeCallFn,
  layerHandle: number
): Promise<number[]> {
  const artHandles: number[] = []
  let current = await GetFirstArtOfLayer(layerHandle)

  while (current !== 0) {
    artHandles.push(current)
    current = await GetArtSibling(current)
  }

  return artHandles
}

/**
 * Get all layers in the document with their handles and display names.
 *
 * @param _bridge - Bridge callable (unused; included for API consistency)
 * @returns Array of layer info objects in document order
 */
export async function getAllLayers(
  _bridge: BridgeCallFn
): Promise<LayerInfo[]> {
  const count = await CountLayers()
  const layers: LayerInfo[] = []

  for (let i = 0; i < count; i++) {
    const handle = await GetNthLayer(i)
    const name = await GetLayerTitle(handle)
    layers.push({ handle, name })
  }

  return layers
}
