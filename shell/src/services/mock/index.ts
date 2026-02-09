/**
 * Mock registry combining all mock modules
 *
 * Central export point for all mock data and utilities.
 */

// Re-export all mock modules
export * from './AIArtMock';
export * from './AILayerMock';

// Import for reset utility
import { resetMockArtState } from './AIArtMock';
import { resetMockLayerState } from './AILayerMock';

/**
 * Mock document metadata
 */
export const mockDocumentInfo = {
  name: 'Untitled-1.ai',
  width: 800,
  height: 600,
  layerCount: 5,
};

/**
 * Mock health response
 */
export const mockHealthResponse = {
  status: 'ok' as const,
  version: '1.0.0-mock',
};

/**
 * Reset all mock state to initial values
 * Useful for testing or reinitializing the mock environment
 */
export function resetAllMockState(): void {
  resetMockArtState();
  resetMockLayerState();
}

/**
 * Simulate network delay for realistic behavior
 * @param ms - Delay in milliseconds (default: 50)
 */
export function simulateDelay(ms: number = 50): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms));
}
