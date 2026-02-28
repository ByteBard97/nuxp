/**
 * SymbolManagementService
 *
 * Manages Illustrator symbol creation, lookup, batch operations, and
 * asset-based symbol import. Uses the bridge-first pattern -- all host
 * interaction goes through the injected BridgeCallFn.
 */

import type { BridgeCallFn } from '../primitives/types'

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

/**
 * Symbol definition for batch creation.
 *
 * Units are intentionally left generic -- the caller is responsible for
 * converting to whatever coordinate system the host plugin expects (e.g.
 * points, millimetres, inches).
 */
export interface SymbolDefinition {
  /** Unique name for the symbol */
  name: string
  /** Fill colour (CSS hex string, e.g. "#4CAF50") */
  color: string
  /** Radius in the unit system of the host (typically points) */
  radius: number
}

/** Result of a single symbol creation request. */
export interface SymbolCreationResult {
  symbolName: string
  created: boolean
}

/** Aggregate result of a batch symbol creation. */
export interface BatchSymbolResult {
  success: boolean
  created: number
  skipped: number
  total: number
}

/** Descriptor for a symbol that already exists in the document. */
export interface SymbolInfo {
  name: string
  type: 'svg' | 'raster' | 'circle'
}

/**
 * Options for importing an external file as a symbol.
 */
export interface ImportAssetOptions {
  /** Whether to convert the imported art to a symbol (default: true) */
  asSymbol?: boolean
  /** Custom name for the symbol (default: derived from filename) */
  symbolName?: string
  /** Scale factor to apply on import (default: 1.0) */
  scale?: number
  /** X position for placement in points (optional) */
  x?: number
  /** Y position for placement in points (optional) */
  y?: number
}

/** Result of an asset import operation. */
export interface ImportAssetResult {
  success: boolean
  /** Handle to the placed art item (if placement succeeded) */
  artHandle?: string
  /** Name of the created symbol (if asSymbol was true) */
  symbolName?: string
  /** Error message if import failed */
  error?: string
}

// ---------------------------------------------------------------------------
// Service
// ---------------------------------------------------------------------------

export class SymbolManagementService {
  constructor(private bridge: BridgeCallFn) {}

  /**
   * Ensure a primitive (circle) symbol exists in the document, creating it
   * if necessary.
   *
   * @param name   - Unique symbol name.
   * @param color  - Fill colour as a CSS hex string.
   * @param radius - Radius in host units (typically points).
   * @returns Whether the symbol already existed or was newly created.
   */
  async ensureSymbol(
    name: string,
    color: string,
    radius: number
  ): Promise<SymbolCreationResult> {
    return this.bridge<SymbolCreationResult>('ensureSymbol', name, color, radius)
  }

  /**
   * Create a new symbol definition in the document.
   *
   * @param name   - Unique symbol name.
   * @param color  - Fill colour as a CSS hex string.
   * @param radius - Radius in host units (typically points).
   */
  async createSymbol(
    name: string,
    color: string,
    radius: number
  ): Promise<SymbolCreationResult> {
    return this.bridge<SymbolCreationResult>('createSymbol', name, color, radius)
  }

  /**
   * Batch-create multiple symbol definitions in a single bridge call.
   *
   * @param symbols - Array of symbol definitions to create.
   */
  async batchCreateSymbols(symbols: SymbolDefinition[]): Promise<BatchSymbolResult> {
    return this.bridge<BatchSymbolResult>('batchCreateSymbols', symbols)
  }

  /**
   * List all symbols currently defined in the active document.
   */
  async getSymbolList(): Promise<SymbolInfo[]> {
    const result = await this.bridge<{ symbols: SymbolInfo[] }>('getSymbolList')
    return result.symbols ?? []
  }

  /**
   * Import an external file (SVG, PNG, etc.) as a symbol in the document.
   *
   * The host plugin is responsible for reading the file from the provided
   * absolute path and placing it into the document.
   *
   * @param filePath - Absolute filesystem path to the asset.
   * @param options  - Import options (symbol name, scale, position).
   */
  async importAssetAsSymbol(
    filePath: string,
    options: ImportAssetOptions = {}
  ): Promise<ImportAssetResult> {
    const {
      asSymbol = true,
      symbolName,
      scale = 1.0,
      x,
      y
    } = options

    return this.bridge<ImportAssetResult>(
      'importAssetAsSymbol',
      filePath,
      asSymbol,
      symbolName ?? '',
      scale,
      x ?? 0,
      y ?? 0
    )
  }
}
