/**
 * DocumentAdapter
 *
 * Bridge-first adapter for document-level Illustrator operations.
 * Each function accepts a `BridgeCallFn` as its first parameter, keeping
 * this module free of any global singleton or framework dependency.
 *
 * Generic operations only -- no application-specific logic.
 */

import type { BridgeCallFn } from '../primitives/types'

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

/** Ruler unit names supported by Illustrator. */
export type UnitName = 'inches' | 'points' | 'millimeters' | 'centimeters' | 'picas'

/** Current document ruler-unit information. */
export interface DocumentUnits {
  /** Numeric unit constant used internally by Illustrator */
  units: number
  /** Human-readable unit name */
  unitsName: UnitName
}

/** Comprehensive document metadata returned by `getDocumentInfo`. */
export interface DocumentInfo {
  /** Whether a document is currently open */
  hasDocument: boolean
  /** Filename without extension */
  name: string
  /** Filename with extension */
  fullName: string
  /** Directory containing the document */
  path: string
  /** Full file path */
  filePath: string
  /** Whether the document has been saved and is unmodified */
  saved: boolean
  /** Total number of artboards */
  artboardCount: number
  /** Index of the currently active artboard */
  currentArtboard: number
  /** Artboard bounding rectangle */
  bounds: {
    left: number
    top: number
    right: number
    bottom: number
  }
  /** Document width in inches */
  width: number
  /** Document height in inches */
  height: number
  /** Document width in points */
  widthPoints: number
  /** Document height in points */
  heightPoints: number
  /** Optional error description if the query partially failed */
  error?: string
}

/** Font descriptor returned by `getAvailableFonts`. */
export interface FontInfo {
  name: string
  family: string
  style: string
  postScriptName?: string
}

/** Result shape for `getAvailableFonts`. */
export interface AvailableFontsResult {
  success: boolean
  fonts: FontInfo[]
  count: number
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

/**
 * Get metadata about the currently open document.
 *
 * Returns filename, path, dimensions, artboard info, and saved status.
 *
 * @param bridge - Bridge callable for host communication
 */
export async function getDocumentInfo(bridge: BridgeCallFn): Promise<DocumentInfo> {
  return bridge<DocumentInfo>('getDocumentInfo')
}

/**
 * Get the current document ruler units.
 *
 * @param bridge - Bridge callable for host communication
 */
export async function getUnits(bridge: BridgeCallFn): Promise<DocumentUnits> {
  return bridge<DocumentUnits>('getDocumentUnits')
}

/**
 * Set the document ruler units.
 *
 * @param bridge - Bridge callable for host communication
 * @param unitsName - Target unit system
 */
export async function setUnits(bridge: BridgeCallFn, unitsName: UnitName): Promise<DocumentUnits> {
  return bridge<DocumentUnits>('setDocumentUnits', unitsName)
}

/**
 * List all fonts available in Illustrator.
 *
 * @param bridge - Bridge callable for host communication
 */
export async function getAvailableFonts(bridge: BridgeCallFn): Promise<AvailableFontsResult> {
  return bridge<AvailableFontsResult>('getAvailableFonts')
}

/**
 * Get artboard dimensions and position for the active artboard.
 *
 * Note: The `ArtboardInfo` type from `../primitives/types` is also usable here;
 * this function returns the same shape the C++ side provides.
 *
 * @param bridge - Bridge callable for host communication
 */
export async function getArtboardInfo(bridge: BridgeCallFn): Promise<DocumentInfo['bounds'] & {
  artboardWidth: number
  artboardHeight: number
}> {
  return bridge('getArtboardInfo')
}
