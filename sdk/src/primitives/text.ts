/**
 * Text and path primitives for the Illustrator bridge.
 *
 * Each function accepts a `bridge` callable as its first argument, making these
 * operations independent of any specific bridge implementation. Consumers pass
 * in their own bridge function (e.g. from HostBridge.callHost).
 */

import type { PathSegment, BridgeCallFn } from './types'

/**
 * Set the PostScript font name for a text frame.
 *
 * @param bridge - Bridge callable for host communication
 * @param uuid - UUID of the text frame
 * @param fontName - PostScript font name to apply
 * @returns Success status, UUID, and applied font name
 */
export async function setTextFont(
  bridge: BridgeCallFn,
  uuid: string,
  fontName: string
): Promise<{ success: boolean; uuid: string; fontName: string }> {
  return bridge('setTextFont', uuid, { fontName })
}

/**
 * Set the font size (in points) for a text frame.
 *
 * @param bridge - Bridge callable for host communication
 * @param uuid - UUID of the text frame
 * @param fontSize - Font size in points
 * @returns Success status, UUID, and applied font size
 */
export async function setTextFontSize(
  bridge: BridgeCallFn,
  uuid: string,
  fontSize: number
): Promise<{ success: boolean; uuid: string; fontSize: number }> {
  return bridge('setTextFontSize', uuid, { fontSize })
}

/**
 * Set the text content of a text frame.
 *
 * @param bridge - Bridge callable for host communication
 * @param uuid - UUID of the text frame
 * @param text - New text content
 * @returns Success status, UUID, and applied text
 */
export async function setTextContent(
  bridge: BridgeCallFn,
  uuid: string,
  text: string
): Promise<{ success: boolean; uuid: string; text: string }> {
  return bridge('setTextContent', uuid, { text })
}

/**
 * Get Bezier segment data for a path art object.
 *
 * @param bridge - Bridge callable for host communication
 * @param uuid - UUID of the path art object
 * @returns Path info including closure state, segment count, and segment data
 */
export async function getPathSegments(
  bridge: BridgeCallFn,
  uuid: string
): Promise<{
  uuid: string
  closed: boolean
  segmentCount: number
  segments: PathSegment[]
}> {
  return bridge('getPathSegments', uuid)
}
