/**
 * PlacementAdapter
 *
 * Bridge-first adapter for interactive placement and symbol-instance
 * operations in Illustrator.
 *
 * Each function accepts a `BridgeCallFn` as its first parameter, keeping
 * this module free of any global singleton or framework dependency.
 *
 * Generic operations only -- no application-specific orchestration.
 */

import type { BridgeCallFn } from '../primitives/types'

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

/** 2-D point in Illustrator document coordinates. */
export interface PlacementPoint {
  x: number
  y: number
}

/** Result returned after a symbol instance is placed on the artboard. */
export interface PlacementResult {
  /** Whether placement succeeded */
  success: boolean
  /** UUID assigned to the placed art object */
  uuid: string
  /** Bounding rectangle of the placed art */
  bounds?: {
    left: number
    top: number
    right: number
    bottom: number
  }
  /** Any additional properties the host may attach */
  [key: string]: unknown
}

/** Options for `placeSymbolInstance`. */
export interface PlaceSymbolOptions {
  /** Scale factor (1.0 = 100 %) */
  scale?: number
  /** Rotation in degrees */
  rotation?: number
  /** Radius in points (used for circle-based symbols) */
  radiusPts?: number
  /** Fill colour as a hex string */
  color?: string
  /** Opacity 0 -- 100 */
  opacity?: number
  /** Fill style for circle symbols */
  circleFillStyle?: 'filled' | 'outline'
  /** Whether to skip artboard bounds validation */
  skipBoundsCheck?: boolean
}

/** Options for `importAsset`. */
export interface ImportAssetOptions {
  /** Position to place the imported art */
  position?: PlacementPoint
  /** Desired width in points (preserves aspect ratio if height omitted) */
  width?: number
  /** Desired height in points */
  height?: number
}

// ---------------------------------------------------------------------------
// Interactive placement workflow
// ---------------------------------------------------------------------------

/**
 * Begin interactive placement mode.
 *
 * Creates a draggable preview on the artboard that follows the cursor.
 *
 * @param bridge      - Bridge callable for host communication
 * @param symbolName  - Name of the symbol to preview
 * @param radiusPts   - Preview circle radius in points
 * @param color       - Preview fill colour (hex string)
 */
export async function startPlacementPreview(
  bridge: BridgeCallFn,
  symbolName: string,
  radiusPts: number,
  color: string,
): Promise<void> {
  await bridge('startPlacementPreview', symbolName, radiusPts, color)
}

/**
 * Update the position of the current placement preview.
 *
 * @param bridge   - Bridge callable for host communication
 * @param position - New preview position in document coordinates
 */
export async function updatePlacementPreview(
  bridge: BridgeCallFn,
  position: PlacementPoint,
): Promise<void> {
  await bridge('updatePlacementPreview', position.x, position.y)
}

/**
 * Finalise the current interactive placement at the preview position.
 *
 * @param bridge     - Bridge callable for host communication
 * @param symbolName - Symbol name being placed
 * @param scale      - Scale factor (default 1.0)
 * @param rotation   - Rotation in degrees (default 0)
 * @returns Placement result with UUID and bounds
 */
export async function finishPlacement(
  bridge: BridgeCallFn,
  symbolName: string,
  scale: number = 1.0,
  rotation: number = 0,
): Promise<PlacementResult> {
  return bridge<PlacementResult>('finishPlacement', symbolName, scale, rotation)
}

/**
 * Cancel the current interactive placement and remove the preview.
 *
 * @param bridge - Bridge callable for host communication
 */
export async function cancelPlacement(bridge: BridgeCallFn): Promise<void> {
  await bridge('cancelPlacement')
}

// ---------------------------------------------------------------------------
// Direct placement
// ---------------------------------------------------------------------------

/**
 * Place a symbol instance at the given position.
 *
 * This is the low-level "place it right here" call -- no interactive
 * preview workflow involved.
 *
 * @param bridge     - Bridge callable for host communication
 * @param symbolName - Name of the symbol to instantiate
 * @param position   - Target position in document coordinates
 * @param options    - Optional scale, rotation, colour, etc.
 * @returns Placement result with UUID and bounds
 */
export async function placeSymbolInstance(
  bridge: BridgeCallFn,
  symbolName: string,
  position: PlacementPoint,
  options: PlaceSymbolOptions = {},
): Promise<PlacementResult> {
  const {
    scale = 1.0,
    rotation = 0,
    radiusPts,
    color,
    opacity,
    circleFillStyle = 'filled',
    skipBoundsCheck,
  } = options

  return bridge<PlacementResult>(
    'placeSymbolInstance',
    symbolName,
    position.x,
    position.y,
    scale,
    rotation,
    radiusPts,
    color,
    undefined, // refDesignator -- app-level concern
    undefined, // letter
    undefined, // displayLabel
    opacity,
    undefined, // fontName
    undefined, // fontSize
    skipBoundsCheck,
    circleFillStyle,
  )
}

// ---------------------------------------------------------------------------
// Metadata helpers
// ---------------------------------------------------------------------------

/**
 * Set name and notes metadata on a placed art object.
 *
 * @param bridge - Bridge callable for host communication
 * @param uuid   - UUID of the target art object
 * @param name   - Display name to assign
 * @param notes  - Arbitrary metadata object (serialised to JSON)
 */
export async function setItemMetadata(
  bridge: BridgeCallFn,
  uuid: string,
  name: string,
  notes: Record<string, unknown>,
): Promise<void> {
  await bridge('setItemMetadata', uuid, name, JSON.stringify(notes))
}

// ---------------------------------------------------------------------------
// Asset import
// ---------------------------------------------------------------------------

/**
 * Import an external file (SVG, AI, PDF, etc.) as placed art.
 *
 * @param bridge   - Bridge callable for host communication
 * @param filePath - Absolute path to the file to import
 * @param options  - Optional position and sizing overrides
 */
export async function importAsset(
  bridge: BridgeCallFn,
  filePath: string,
  options: ImportAssetOptions = {},
): Promise<PlacementResult> {
  return bridge<PlacementResult>('importAsset', filePath, options)
}
