/**
 * Type definitions for NUXP plugin communication
 */

/**
 * Information about an Illustrator document
 */
export interface DocumentInfo {
  /** Document filename */
  name: string;
  /** Document width in points */
  width: number;
  /** Document height in points */
  height: number;
  /** Number of layers in the document */
  layerCount: number;
}

/**
 * Information about a layer in the document
 */
export interface LayerInfo {
  /** Unique layer identifier */
  id: number;
  /** Layer name as displayed in Layers panel */
  name: string;
  /** Whether the layer is visible */
  visible: boolean;
  /** Whether the layer is locked */
  locked: boolean;
}

/**
 * Bounding box for art items
 */
export interface ArtBounds {
  left: number;
  top: number;
  right: number;
  bottom: number;
}

/**
 * Information about an art item (path, group, text, etc.)
 */
export interface ArtInfo {
  /** Unique art item identifier */
  id: number;
  /** Art type (e.g., 'path', 'group', 'text', 'compound') */
  type: string;
  /** Art item name (may be empty) */
  name: string;
  /** Bounding box in document coordinates */
  bounds: ArtBounds;
}

/**
 * Plugin health check response
 */
export interface HealthResponse {
  /** Plugin status */
  status: 'ok' | 'error';
  /** Plugin version string */
  version: string;
  /** Optional error message */
  message?: string;
}

/**
 * Generic API response wrapper
 */
export interface ApiResponse<T> {
  /** Whether the request succeeded */
  success: boolean;
  /** Response data if successful */
  data?: T;
  /** Error message if failed */
  error?: string;
}
