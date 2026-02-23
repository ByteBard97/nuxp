/**
 * Shared type definitions for host application art primitives.
 *
 * These types describe the data structures returned by the C++ bridge layer
 * when querying or manipulating art objects in Adobe Illustrator.
 */

/**
 * Bounding rectangle of an art object in Illustrator coordinates.
 */
export interface ArtBounds {
  left: number
  top: number
  right: number
  bottom: number
}

/**
 * A direct child entry within a group art object.
 */
export interface ArtChild {
  /** Display name of the child art object */
  name: string
  /** Numeric art type constant (see ArtType) */
  artType: number
  /** Whether the child is currently visible */
  visible: boolean
  /** UUID of the child, if available */
  uuid?: string
}

/**
 * A single Bezier path segment with anchor, in-handle, and out-handle control points.
 */
export interface PathSegment {
  /** Anchor (on-curve) point */
  anchor: { h: number; v: number }
  /** Incoming control handle */
  in: { h: number; v: number }
  /** Outgoing control handle */
  out: { h: number; v: number }
  /** Whether this is a corner point (vs. smooth) */
  corner: boolean
}

/**
 * Information about the active artboard dimensions and position.
 */
export interface ArtboardInfo {
  artboardWidth: number
  artboardHeight: number
  left: number
  top: number
  right: number
  bottom: number
}

/**
 * Illustrator art type constants (from AIArt.h enum, 0-indexed).
 *
 * kUnknownArt=0, kGroupArt=1, kPathArt=2, kCompoundPathArt=3,
 * kTextArtUnsupported=4..6, kPlacedArt=7, kMysteryPathArt=8,
 * kRasterArt=9, kPluginArt=10, kMeshArt=11, kTextFrameArt=12, kSymbolArt=13
 */
export const ArtType = {
  kGroupArt: 1,
  kPathArt: 2,
  kCompoundPathArt: 3,
  kPlacedArt: 7,
  kRasterArt: 9,
  kPluginArt: 10,
  kMeshArt: 11,
  kTextFrameArt: 12,
  kSymbolArt: 13,
} as const

/** Function signature for bridge calls — used by primitive wrappers */
export type BridgeCallFn = <T = unknown>(functionName: string, ...args: unknown[]) => Promise<T>
