/**
 * AIArtSuite mock data for frontend development
 *
 * Provides realistic fake art item data allowing UI development
 * without the C++ plugin running.
 */

import type { ArtInfo, ArtBounds } from '../types';

/** Next available handle ID for art items */
let nextArtHandleId = 1001;

/**
 * Generate a new unique art handle ID
 */
export function generateArtHandleId(): number {
  return nextArtHandleId++;
}

/**
 * Reset the art handle ID counter (useful for testing)
 */
export function resetArtHandleIds(): void {
  nextArtHandleId = 1001;
}

/**
 * Art item types supported by Illustrator
 */
export type ArtType =
  | 'path'
  | 'compound'
  | 'group'
  | 'text'
  | 'raster'
  | 'placed'
  | 'symbol'
  | 'mesh'
  | 'plugin'
  | 'unknown';

/**
 * Create a mock art item with specified properties
 */
export function createMockArt(
  type: ArtType,
  name: string,
  bounds: ArtBounds,
  id?: number
): ArtInfo {
  return {
    id: id ?? generateArtHandleId(),
    type,
    name,
    bounds,
  };
}

/**
 * Pre-defined mock art items representing typical document content
 */
export const mockArtItems: ArtInfo[] = [
  createMockArt('path', 'Logo Shape', {
    left: 100,
    top: 500,
    right: 300,
    bottom: 400,
  }),
  createMockArt('text', 'Title Text', {
    left: 350,
    top: 520,
    right: 550,
    bottom: 480,
  }),
  createMockArt('group', 'Icon Group', {
    left: 150,
    top: 350,
    right: 250,
    bottom: 250,
  }),
  createMockArt('path', 'Background Rectangle', {
    left: 0,
    top: 600,
    right: 800,
    bottom: 0,
  }),
  createMockArt('compound', 'Complex Path', {
    left: 400,
    top: 300,
    right: 550,
    bottom: 150,
  }),
];

/**
 * Mock selection state - which art items are currently selected
 */
let mockSelectedIds: Set<number> = new Set([mockArtItems[0].id]);

/**
 * Get the currently selected art items
 */
export function getMockSelection(): ArtInfo[] {
  return mockArtItems.filter((art) => mockSelectedIds.has(art.id));
}

/**
 * Set the mock selection by art item IDs
 */
export function setMockSelection(ids: number[]): boolean {
  const validIds = ids.filter((id) => mockArtItems.some((art) => art.id === id));
  mockSelectedIds = new Set(validIds);
  return true;
}

/**
 * Clear the current selection
 */
export function clearMockSelection(): void {
  mockSelectedIds.clear();
}

/**
 * Get all mock art items in the document
 */
export function getAllMockArt(): ArtInfo[] {
  return [...mockArtItems];
}

/**
 * Add a new mock art item to the document
 */
export function addMockArt(art: ArtInfo): void {
  mockArtItems.push(art);
}

/**
 * Reset the mock art state to defaults (useful for testing)
 */
export function resetMockArtState(): void {
  resetArtHandleIds();
  mockArtItems.length = 0;
  mockArtItems.push(
    createMockArt('path', 'Logo Shape', {
      left: 100,
      top: 500,
      right: 300,
      bottom: 400,
    }),
    createMockArt('text', 'Title Text', {
      left: 350,
      top: 520,
      right: 550,
      bottom: 480,
    }),
    createMockArt('group', 'Icon Group', {
      left: 150,
      top: 350,
      right: 250,
      bottom: 250,
    }),
    createMockArt('path', 'Background Rectangle', {
      left: 0,
      top: 600,
      right: 800,
      bottom: 0,
    }),
    createMockArt('compound', 'Complex Path', {
      left: 400,
      top: 300,
      right: 550,
      bottom: 150,
    })
  );
  mockSelectedIds = new Set([mockArtItems[0].id]);
}
