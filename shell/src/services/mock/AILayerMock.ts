/**
 * AILayerSuite mock data for frontend development
 *
 * Provides realistic fake layer data allowing UI development
 * without the C++ plugin running.
 */

import type { LayerInfo } from '../types';

/** Next available handle ID for layers */
let nextLayerHandleId = 101;

/**
 * Generate a new unique layer handle ID
 */
export function generateLayerHandleId(): number {
  return nextLayerHandleId++;
}

/**
 * Reset the layer handle ID counter (useful for testing)
 */
export function resetLayerHandleIds(): void {
  nextLayerHandleId = 101;
}

/**
 * Mutable mock layer state
 */
interface MockLayerState extends LayerInfo {
  // Can extend with additional mock-only properties if needed
}

/**
 * Pre-defined mock layers representing typical document structure
 */
const mockLayers: MockLayerState[] = [
  {
    id: 1,
    name: 'Layer 1',
    visible: true,
    locked: false,
  },
  {
    id: 2,
    name: 'Background',
    visible: true,
    locked: true,
  },
  {
    id: 3,
    name: 'Guides',
    visible: false,
    locked: true,
  },
  {
    id: 4,
    name: 'Icons',
    visible: true,
    locked: false,
  },
  {
    id: 5,
    name: 'Text',
    visible: true,
    locked: false,
  },
];

/**
 * Get all mock layers
 */
export function getMockLayers(): LayerInfo[] {
  return mockLayers.map((layer) => ({ ...layer }));
}

/**
 * Get a specific layer by ID
 */
export function getMockLayerById(id: number): LayerInfo | null {
  const layer = mockLayers.find((l) => l.id === id);
  return layer ? { ...layer } : null;
}

/**
 * Set layer visibility
 */
export function setMockLayerVisibility(layerId: number, visible: boolean): boolean {
  const layer = mockLayers.find((l) => l.id === layerId);
  if (!layer) {
    return false;
  }
  layer.visible = visible;
  return true;
}

/**
 * Set layer locked state
 */
export function setMockLayerLocked(layerId: number, locked: boolean): boolean {
  const layer = mockLayers.find((l) => l.id === layerId);
  if (!layer) {
    return false;
  }
  layer.locked = locked;
  return true;
}

/**
 * Create a new mock layer
 */
export function createMockLayer(name: string, options?: Partial<Omit<LayerInfo, 'id' | 'name'>>): LayerInfo {
  const newLayer: MockLayerState = {
    id: generateLayerHandleId(),
    name,
    visible: options?.visible ?? true,
    locked: options?.locked ?? false,
  };
  mockLayers.unshift(newLayer); // Add to top of layer stack
  return { ...newLayer };
}

/**
 * Delete a mock layer by ID
 */
export function deleteMockLayer(id: number): boolean {
  const index = mockLayers.findIndex((l) => l.id === id);
  if (index === -1) {
    return false;
  }
  mockLayers.splice(index, 1);
  return true;
}

/**
 * Rename a mock layer
 */
export function renameMockLayer(id: number, newName: string): boolean {
  const layer = mockLayers.find((l) => l.id === id);
  if (!layer) {
    return false;
  }
  layer.name = newName;
  return true;
}

/**
 * Reset the mock layer state to defaults (useful for testing)
 */
export function resetMockLayerState(): void {
  resetLayerHandleIds();
  mockLayers.length = 0;
  mockLayers.push(
    {
      id: 1,
      name: 'Layer 1',
      visible: true,
      locked: false,
    },
    {
      id: 2,
      name: 'Background',
      visible: true,
      locked: true,
    },
    {
      id: 3,
      name: 'Guides',
      visible: false,
      locked: true,
    },
    {
      id: 4,
      name: 'Icons',
      visible: true,
      locked: false,
    },
    {
      id: 5,
      name: 'Text',
      visible: true,
      locked: false,
    }
  );
}
