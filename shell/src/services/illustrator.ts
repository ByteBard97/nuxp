/**
 * Illustrator-specific API calls
 *
 * This module provides high-level functions for interacting with
 * Adobe Illustrator through the NUXP plugin's HTTP API.
 */

import { callPlugin, getFromPlugin, ApiError } from './api';
import type { DocumentInfo, LayerInfo, ArtInfo } from './types';

/**
 * Response structure for document info endpoint
 */
interface DocumentResponse {
  document: DocumentInfo | null;
}

/**
 * Response structure for layers endpoint
 */
interface LayersResponse {
  layers: LayerInfo[];
}

/**
 * Response structure for selection endpoint
 */
interface SelectionResponse {
  selection: ArtInfo[];
}

/**
 * Get information about the currently active document
 *
 * @returns DocumentInfo if a document is open, null otherwise
 */
export async function getActiveDocument(): Promise<DocumentInfo | null> {
  try {
    const response = await getFromPlugin<DocumentResponse>('/document/info');
    if (response.success && response.data?.document) {
      return response.data.document;
    }
    return null;
  } catch (error) {
    if (error instanceof ApiError) {
      console.error(`Failed to get active document: ${error.message}`);
    }
    return null;
  }
}

/**
 * Get all layers in the current document
 *
 * @returns Array of LayerInfo objects, empty array if no document or error
 */
export async function getDocumentLayers(): Promise<LayerInfo[]> {
  try {
    const response = await getFromPlugin<LayersResponse>('/document/layers');
    if (response.success && response.data?.layers) {
      return response.data.layers;
    }
    return [];
  } catch (error) {
    if (error instanceof ApiError) {
      console.error(`Failed to get document layers: ${error.message}`);
    }
    return [];
  }
}

/**
 * Get the currently selected art items
 *
 * @returns Array of ArtInfo objects for selected items, empty array if none or error
 */
export async function getSelection(): Promise<ArtInfo[]> {
  try {
    const response = await getFromPlugin<SelectionResponse>('/selection/info');
    if (response.success && response.data?.selection) {
      return response.data.selection;
    }
    return [];
  } catch (error) {
    if (error instanceof ApiError) {
      console.error(`Failed to get selection: ${error.message}`);
    }
    return [];
  }
}

/**
 * Select art items by their IDs
 *
 * @param ids - Array of art item IDs to select
 * @returns true if selection was successful, false otherwise
 */
export async function selectArtItems(ids: number[]): Promise<boolean> {
  try {
    const response = await callPlugin<{ success: boolean }>('/selection/set', { ids });
    return response.success;
  } catch (error) {
    if (error instanceof ApiError) {
      console.error(`Failed to set selection: ${error.message}`);
    }
    return false;
  }
}

/**
 * Get a specific layer by ID
 *
 * @param layerId - The layer ID to retrieve
 * @returns LayerInfo if found, null otherwise
 */
export async function getLayerById(layerId: number): Promise<LayerInfo | null> {
  try {
    const response = await getFromPlugin<{ layer: LayerInfo | null }>(`/layer/${layerId}`);
    if (response.success && response.data?.layer) {
      return response.data.layer;
    }
    return null;
  } catch (error) {
    if (error instanceof ApiError) {
      console.error(`Failed to get layer ${layerId}: ${error.message}`);
    }
    return null;
  }
}

/**
 * Toggle layer visibility
 *
 * @param layerId - The layer ID to toggle
 * @param visible - Whether the layer should be visible
 * @returns true if operation succeeded, false otherwise
 */
export async function setLayerVisibility(layerId: number, visible: boolean): Promise<boolean> {
  try {
    const response = await callPlugin<{ success: boolean }>('/layer/visibility', {
      layerId,
      visible,
    });
    return response.success;
  } catch (error) {
    if (error instanceof ApiError) {
      console.error(`Failed to set layer visibility: ${error.message}`);
    }
    return false;
  }
}

/**
 * Toggle layer lock state
 *
 * @param layerId - The layer ID to toggle
 * @param locked - Whether the layer should be locked
 * @returns true if operation succeeded, false otherwise
 */
export async function setLayerLocked(layerId: number, locked: boolean): Promise<boolean> {
  try {
    const response = await callPlugin<{ success: boolean }>('/layer/lock', {
      layerId,
      locked,
    });
    return response.success;
  } catch (error) {
    if (error instanceof ApiError) {
      console.error(`Failed to set layer lock state: ${error.message}`);
    }
    return false;
  }
}

/**
 * Execute a raw plugin command (for debugging)
 *
 * @param command - Command name
 * @param params - Command parameters
 * @returns Raw response data
 */
export async function executeCommand(command: string, params?: unknown): Promise<unknown> {
  try {
    const response = await callPlugin<unknown>('/command/execute', {
      command,
      params,
    });
    return response.data;
  } catch (error) {
    if (error instanceof ApiError) {
      throw error;
    }
    throw new Error('Unknown error executing command');
  }
}
