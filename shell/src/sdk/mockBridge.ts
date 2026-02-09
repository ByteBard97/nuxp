/**
 * Mock Bridge for SDK
 *
 * Provides mock implementations for callCpp when running in mock mode.
 * This integrates with the existing mock infrastructure in services/mock/
 * to provide realistic fake data for frontend development.
 */

import { simulateDelay } from '../services/mock';

/**
 * Check if mock mode is enabled via environment
 */
export function isMockModeEnabled(): boolean {
  return import.meta.env.VITE_USE_MOCK === 'true';
}

/**
 * Mock handler function type
 */
type MockHandler = (args: Record<string, unknown>) => unknown | Promise<unknown>;

/**
 * Registry of mock handlers organized by suite and method
 */
const mockHandlers: Map<string, Map<string, MockHandler>> = new Map();

/**
 * Register a mock handler for a suite method
 *
 * @param suite - Suite name (e.g., "AIArt")
 * @param method - Method name (e.g., "GetArtName")
 * @param handler - Handler function that returns mock data
 */
export function registerMockHandler(
  suite: string,
  method: string,
  handler: MockHandler
): void {
  if (!mockHandlers.has(suite)) {
    mockHandlers.set(suite, new Map());
  }
  mockHandlers.get(suite)!.set(method, handler);
}

/**
 * Get a mock handler for a suite method
 *
 * @param suite - Suite name
 * @param method - Method name
 * @returns The handler if registered, undefined otherwise
 */
function getMockHandler(suite: string, method: string): MockHandler | undefined {
  return mockHandlers.get(suite)?.get(method);
}

/**
 * Mock implementation of callCpp
 *
 * @param suite - Suite name
 * @param method - Method name
 * @param args - Arguments passed to the method
 * @returns Mock result data
 */
export async function mockCallCpp<T = unknown>(
  suite: string,
  method: string,
  args: Record<string, unknown>
): Promise<T> {
  // Simulate network delay for realistic behavior
  await simulateDelay(30);

  const handler = getMockHandler(suite, method);

  if (handler) {
    const result = await handler(args);
    return result as T;
  }

  // Log warning for unregistered handlers
  console.warn(
    `[MockBridge] No mock handler registered for ${suite}.${method}. ` +
    `Register one with registerMockHandler('${suite}', '${method}', handler).`
  );

  // Return a generic mock response based on method name patterns
  return generateDefaultMockResult<T>(suite, method, args);
}

/**
 * Generate a default mock result based on method naming patterns
 *
 * @param suite - Suite name
 * @param method - Method name
 * @param args - Arguments (may be used for context)
 * @returns A plausible default mock result
 */
function generateDefaultMockResult<T>(
  suite: string,
  method: string,
  _args: Record<string, unknown>
): T {
  // Get methods typically return a value
  if (method.startsWith('Get')) {
    const property = method.slice(3);

    // Common property patterns
    if (property.includes('Name')) {
      return `Mock ${suite} Name` as unknown as T;
    }
    if (property.includes('Type')) {
      return 0 as unknown as T;
    }
    if (property.includes('Count')) {
      return 3 as unknown as T;
    }
    if (property.includes('Bounds') || property.includes('Rect')) {
      return { left: 0, top: 100, right: 100, bottom: 0 } as unknown as T;
    }
    if (property.includes('Point')) {
      return { h: 50, v: 50 } as unknown as T;
    }
    if (property.includes('Matrix')) {
      return { a: 1, b: 0, c: 0, d: 1, tx: 0, ty: 0 } as unknown as T;
    }
    if (property.includes('Visible') || property.includes('Locked') || property.includes('Selected')) {
      return true as unknown as T;
    }

    // Default: return a handle-like number
    return 1001 as unknown as T;
  }

  // Set methods typically return void
  if (method.startsWith('Set')) {
    return undefined as unknown as T;
  }

  // Count methods return numbers
  if (method.startsWith('Count')) {
    return 5 as unknown as T;
  }

  // Boolean check methods
  if (method.startsWith('Is') || method.startsWith('Has') || method.startsWith('Can')) {
    return true as unknown as T;
  }

  // Create/New methods return handles
  if (method.startsWith('Create') || method.startsWith('New')) {
    return 2001 as unknown as T;
  }

  // Delete/Remove methods return void
  if (method.startsWith('Delete') || method.startsWith('Remove') || method.startsWith('Dispose')) {
    return undefined as unknown as T;
  }

  // Default: return undefined (void-like)
  return undefined as unknown as T;
}

/**
 * Clear all registered mock handlers
 * Useful for test setup/teardown
 */
export function clearMockHandlers(): void {
  mockHandlers.clear();
}

/**
 * Get all registered mock handlers (for debugging)
 *
 * @returns Map of suite -> method -> handler
 */
export function getMockHandlerRegistry(): Map<string, Map<string, MockHandler>> {
  return new Map(mockHandlers);
}

// ============================================================================
// Pre-registered mock handlers for common operations
// These provide reasonable defaults for the most common SDK calls
// ============================================================================

// AIArt suite mocks
registerMockHandler('AIArt', 'GetArtName', (args) => {
  const artId = args.art as number;
  return `Art Object ${artId}`;
});

registerMockHandler('AIArt', 'GetArtType', () => {
  // Return kPathArt type
  return 1;
});

registerMockHandler('AIArt', 'GetArtBounds', () => {
  return { left: 10, top: 200, right: 150, bottom: 50 };
});

registerMockHandler('AIArt', 'GetArtParent', (args) => {
  const artId = args.art as number;
  // Return a plausible parent handle (lower number = parent)
  return Math.max(1, artId - 100);
});

registerMockHandler('AIArt', 'SetArtName', () => {
  // Set operations return void
  return undefined;
});

// Counter for generating unique art handles in mock mode
let mockArtHandleCounter = 3000;

registerMockHandler('AIArtSuite', 'NewArt', () => {
  // Return a unique mock art handle
  return { newArt: mockArtHandleCounter++ };
});

registerMockHandler('AIArtSuite', 'SetArtName', () => {
  return undefined;
});

registerMockHandler('AIArtSuite', 'GetArtBounds', () => {
  return { bounds: { left: 100, top: 200, right: 200, bottom: 100 } };
});

// AIPathSuite mock handlers
registerMockHandler('AIPathSuite', 'SetPathSegments', () => {
  return undefined;
});

registerMockHandler('AIPathSuite', 'SetPathClosed', () => {
  return undefined;
});

registerMockHandler('AIPathSuite', 'GetPathSegmentCount', () => {
  return { count: 4 };
});

// AILayer suite mocks
registerMockHandler('AILayer', 'GetLayerTitle', (args) => {
  const layerId = args.layer as number;
  return `Layer ${layerId}`;
});

registerMockHandler('AILayer', 'GetLayerVisible', () => {
  return true;
});

registerMockHandler('AILayer', 'GetLayerEditable', () => {
  return true;
});

registerMockHandler('AILayer', 'SetLayerVisible', () => {
  return undefined;
});

registerMockHandler('AILayer', 'SetLayerEditable', () => {
  return undefined;
});

registerMockHandler('AILayer', 'CountLayers', () => {
  return 5;
});

registerMockHandler('AILayer', 'GetNthLayer', (args) => {
  const index = args.n as number;
  return 1000 + index;
});

// AIDocument suite mocks
registerMockHandler('AIDocument', 'GetDocumentName', () => {
  return 'Untitled-1.ai';
});

registerMockHandler('AIDocument', 'GetDocumentWidth', () => {
  return 800;
});

registerMockHandler('AIDocument', 'GetDocumentHeight', () => {
  return 600;
});
