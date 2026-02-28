/**
 * SvgPlacementService
 *
 * Handles SVG-specific placement operations: loading SVG content from a
 * cached path, validating its structure, and placing it on the canvas via
 * the bridge.
 */

import type { BridgeCallFn } from '../primitives/types'
import type { Point } from '../types/core'

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------

/** Result of a placement operation. */
export interface PlacementResult {
  uuid: string
  bounds: {
    left: number
    top: number
    width: number
    height: number
  }
}

/** Metadata describing how an SVG should be placed. */
export interface SvgMetadata {
  /** Absolute filesystem path to the SVG file */
  path: string
  /** Uniform scale factor (1.0 = no scaling) */
  scale: number
  /** Horizontal offset from the placement anchor */
  offsetX: number
  /** Vertical offset from the placement anchor */
  offsetY: number
}

/** Options passed to the host for SVG placement. */
export interface SvgPlacementOptions {
  /** Symbol name to register or look up */
  symbolName: string
  /** Canvas position */
  position: Point
  /** Radius of the backing circle, in host units (points) */
  radiusPts: number
  /** Fill colour for the backing circle (CSS hex) */
  color: string
  /** Optional label / reference designator */
  label?: string
  /** Display label (may differ from internal label) */
  displayLabel?: string
  /** Opacity 0..100 */
  opacity?: number
  /** Font family for the label */
  fontName?: string
  /** Font size for the label (points) */
  fontSize?: number
  /** Path to the SVG file on disk */
  svgPath?: string
  /** Scale factor for the SVG */
  svgScale?: number
  /** SVG horizontal offset */
  svgOffsetX?: number
  /** SVG vertical offset */
  svgOffsetY?: number
  /** Circle fill style */
  circleFillStyle?: 'filled' | 'outline'
  /** Whether to draw a stroke on the circle */
  strokeEnabled?: boolean
  /** Stroke colour (CSS hex) */
  strokeColor?: string
  /** Stroke width in points */
  strokeWidth?: number
}

/** Result of SVG content validation. */
export interface SvgValidationResult {
  valid: boolean
  errors: string[]
  warnings: string[]
  hasViewBox: boolean
  width?: number
  height?: number
}

// ---------------------------------------------------------------------------
// Service
// ---------------------------------------------------------------------------

export class SvgPlacementService {
  constructor(private bridge: BridgeCallFn) {}

  /**
   * Place an SVG instance (backed by a circle symbol) at the given position.
   *
   * All parameters are forwarded to the host plugin's `placeWithSvg` handler.
   */
  async placeWithSvg(opts: SvgPlacementOptions): Promise<PlacementResult> {
    return this.bridge<PlacementResult>(
      'placeWithSvg',
      opts.symbolName,
      opts.position.x,
      opts.position.y,
      opts.radiusPts,
      opts.color,
      opts.label ?? '',
      opts.displayLabel ?? opts.label ?? '',
      opts.opacity ?? 100,
      opts.fontName ?? 'Arial',
      opts.fontSize ?? 12,
      opts.svgPath ?? '',
      opts.svgScale ?? 1.0,
      opts.svgOffsetX ?? 0,
      opts.svgOffsetY ?? 0,
      opts.circleFillStyle ?? 'filled',
      opts.strokeEnabled ?? true,
      opts.strokeColor ?? '#000000',
      opts.strokeWidth ?? 1.0
    )
  }

  /**
   * Validate SVG content for placement compatibility with Illustrator.
   *
   * Checks for a root `<svg>` element, viewBox, dimensions, and warns about
   * potentially problematic features (scripts, foreignObject, embedded
   * images, filters).
   *
   * This is a pure function -- it does not call the bridge.
   */
  validateSvgContent(svgContent: string): SvgValidationResult {
    const result: SvgValidationResult = {
      valid: true,
      errors: [],
      warnings: [],
      hasViewBox: false
    }

    // Empty content
    if (!svgContent || svgContent.trim().length === 0) {
      result.valid = false
      result.errors.push('SVG content is empty')
      return result
    }

    // Root element
    if (!svgContent.includes('<svg')) {
      result.valid = false
      result.errors.push('Content does not contain SVG root element')
      return result
    }

    // viewBox
    const viewBoxMatch = svgContent.match(/viewBox\s*=\s*["']([^"']+)["']/)
    if (viewBoxMatch) {
      result.hasViewBox = true
      const parts = viewBoxMatch[1].split(/\s+/).map(Number)
      if (parts.length === 4) {
        result.width = parts[2]
        result.height = parts[3]
      }
    } else {
      result.warnings.push('SVG does not have a viewBox attribute - scaling may be inconsistent')
    }

    // Fallback dimensions
    if (!result.hasViewBox) {
      const widthMatch = svgContent.match(/width\s*=\s*["']([^"']+)["']/)
      const heightMatch = svgContent.match(/height\s*=\s*["']([^"']+)["']/)

      if (widthMatch && heightMatch) {
        result.width = parseFloat(widthMatch[1])
        result.height = parseFloat(heightMatch[1])
      } else {
        result.warnings.push('SVG has no viewBox or dimensions - sizing may be unpredictable')
      }
    }

    // Problematic features
    if (svgContent.includes('<script')) {
      result.warnings.push('SVG contains script elements which may not render correctly')
    }
    if (svgContent.includes('<foreignObject')) {
      result.warnings.push('SVG contains foreignObject elements which may not render correctly in Illustrator')
    }
    if (svgContent.includes('xlink:href') || svgContent.includes('href="data:image')) {
      result.warnings.push('SVG contains embedded images which may affect performance')
    }
    if (svgContent.includes('<filter')) {
      result.warnings.push('SVG contains filters which may render differently in Illustrator')
    }

    return result
  }
}
