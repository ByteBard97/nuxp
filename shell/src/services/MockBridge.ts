/**
 * Mock API Bridge for frontend development
 *
 * Intercepts plugin API calls when VITE_USE_MOCK=true, returning
 * realistic fake data with simulated network delay. This allows
 * frontend development without the C++ plugin running.
 */

import type { ApiResponse, DocumentInfo, LayerInfo, ArtInfo, HealthResponse } from './types';
import {
  mockDocumentInfo,
  mockHealthResponse,
  simulateDelay,
  getMockLayers,
  getMockLayerById,
  setMockLayerVisibility,
  setMockLayerLocked,
  getMockSelection,
  setMockSelection,
} from './mock';

/** Default simulated network delay in milliseconds */
const MOCK_DELAY_MS = 50;

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
 * Mock endpoint handlers registry
 */
type MockHandler = (data?: unknown) => Promise<unknown>;

const mockEndpoints: Map<string, MockHandler> = new Map();

/**
 * Register a mock endpoint handler
 */
function registerMockEndpoint(endpoint: string, handler: MockHandler): void {
  // Normalize endpoint to always start with /
  const normalizedEndpoint = endpoint.startsWith('/') ? endpoint : `/${endpoint}`;
  mockEndpoints.set(normalizedEndpoint, handler);
}

/**
 * Initialize all mock endpoint handlers
 */
function initializeMockEndpoints(): void {
  // Health check
  registerMockEndpoint('/health', async (): Promise<HealthResponse> => {
    return mockHealthResponse;
  });

  // Document info
  registerMockEndpoint('/document/info', async (): Promise<DocumentResponse> => {
    return { document: mockDocumentInfo };
  });

  // Document layers
  registerMockEndpoint('/document/layers', async (): Promise<LayersResponse> => {
    return { layers: getMockLayers() };
  });

  // Selection info
  registerMockEndpoint('/selection/info', async (): Promise<SelectionResponse> => {
    return { selection: getMockSelection() };
  });

  // Set selection
  registerMockEndpoint('/selection/set', async (data?: unknown): Promise<{ success: boolean }> => {
    const payload = data as { ids?: number[] } | undefined;
    if (payload?.ids) {
      setMockSelection(payload.ids);
    }
    return { success: true };
  });

  // Layer visibility
  registerMockEndpoint('/layer/visibility', async (data?: unknown): Promise<{ success: boolean }> => {
    const payload = data as { layerId?: number; visible?: boolean } | undefined;
    if (payload?.layerId !== undefined && payload?.visible !== undefined) {
      const result = setMockLayerVisibility(payload.layerId, payload.visible);
      return { success: result };
    }
    return { success: false };
  });

  // Layer lock
  registerMockEndpoint('/layer/lock', async (data?: unknown): Promise<{ success: boolean }> => {
    const payload = data as { layerId?: number; locked?: boolean } | undefined;
    if (payload?.layerId !== undefined && payload?.locked !== undefined) {
      const result = setMockLayerLocked(payload.layerId, payload.locked);
      return { success: result };
    }
    return { success: false };
  });

  // Command execute (generic handler)
  registerMockEndpoint('/command/execute', async (data?: unknown): Promise<{ result: string }> => {
    const payload = data as { command?: string; params?: unknown } | undefined;
    console.log('[MockBridge] Command executed:', payload?.command, payload?.params);
    return { result: 'mock_command_success' };
  });
}

// Initialize endpoints on module load
initializeMockEndpoints();

/**
 * Check if mock mode is enabled
 */
export function isMockMode(): boolean {
  return import.meta.env.VITE_USE_MOCK === 'true';
}

/**
 * Mock implementation of callPlugin
 *
 * @param endpoint - API endpoint path
 * @param data - Optional request body data
 * @returns Mock API response with simulated delay
 */
export async function mockCallPlugin<T = unknown>(
  endpoint: string,
  data?: unknown
): Promise<ApiResponse<T>> {
  await simulateDelay(MOCK_DELAY_MS);

  const normalizedEndpoint = endpoint.startsWith('/') ? endpoint : `/${endpoint}`;

  // Check for dynamic layer endpoint pattern: /layer/:id
  const layerMatch = normalizedEndpoint.match(/^\/layer\/(\d+)$/);
  if (layerMatch) {
    const layerId = parseInt(layerMatch[1], 10);
    const layer = getMockLayerById(layerId);
    return {
      success: true,
      data: { layer } as unknown as T,
    };
  }

  const handler = mockEndpoints.get(normalizedEndpoint);

  if (!handler) {
    console.warn(`[MockBridge] No mock handler for endpoint: ${normalizedEndpoint}`);
    return {
      success: false,
      error: `Mock endpoint not implemented: ${normalizedEndpoint}`,
    };
  }

  try {
    const result = await handler(data);
    return {
      success: true,
      data: result as T,
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown mock error';
    return {
      success: false,
      error: message,
    };
  }
}

/**
 * Mock implementation of getFromPlugin
 *
 * @param endpoint - API endpoint path
 * @returns Mock API response with simulated delay
 */
export async function mockGetFromPlugin<T = unknown>(
  endpoint: string
): Promise<ApiResponse<T>> {
  return mockCallPlugin<T>(endpoint);
}

/**
 * Mock implementation of checkHealth
 */
export async function mockCheckHealth(): Promise<boolean> {
  await simulateDelay(MOCK_DELAY_MS);
  return true;
}

/**
 * Mock implementation of getHealthInfo
 */
export async function mockGetHealthInfo(): Promise<HealthResponse> {
  await simulateDelay(MOCK_DELAY_MS);
  return mockHealthResponse;
}

/**
 * Register a custom mock endpoint handler (for testing or extensions)
 */
export function registerCustomMockEndpoint(endpoint: string, handler: MockHandler): void {
  registerMockEndpoint(endpoint, handler);
}

/**
 * Clear all custom mock endpoints (keeps built-in ones)
 */
export function resetMockEndpoints(): void {
  mockEndpoints.clear();
  initializeMockEndpoints();
}
