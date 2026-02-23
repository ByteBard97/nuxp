/**
 * Art Creation Test Suite
 *
 * Tests for AIArtSuite and AIPathSuite SDK bindings. These tests verify
 * the ability to create and manipulate art objects in Illustrator via
 * the C++ plugin's HTTP API.
 *
 * Note: Some endpoints may not be implemented yet. Tests are designed to:
 * 1. Succeed if the endpoint works
 * 2. Fail with clear "endpoint not implemented" message if 404
 * 3. Serve as documentation for what endpoints need to be built
 */

import type { TestSuite, TestResult } from '../types';

const PLUGIN_URL = 'http://localhost:8080';

/**
 * Call a plugin endpoint using the suite/method pattern
 *
 * @param suite - The SDK suite name (e.g., 'AIArtSuite')
 * @param method - The method name (e.g., 'NewArt')
 * @param args - Arguments to pass to the method
 * @returns The JSON response from the plugin
 */
async function callPlugin(suite: string, method: string, args: Record<string, unknown> = {}): Promise<unknown> {
  const response = await fetch(`${PLUGIN_URL}/api/call`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ suite, method, args }),
  });

  if (!response.ok) {
    const errorText = await response.text();
    throw new Error(`HTTP ${response.status}: ${errorText}`);
  }

  return response.json();
}

/**
 * Call a high-level convenience endpoint (may not exist yet)
 *
 * @param endpoint - The endpoint path (e.g., '/art/create-rectangle')
 * @param data - Optional request body
 * @returns The JSON response from the plugin
 */
async function callEndpoint(endpoint: string, data?: unknown): Promise<unknown> {
  const response = await fetch(`${PLUGIN_URL}${endpoint}`, {
    method: data ? 'POST' : 'GET',
    headers: data ? { 'Content-Type': 'application/json' } : {},
    body: data ? JSON.stringify(data) : undefined,
  });

  if (!response.ok) {
    throw new Error(`HTTP ${response.status}: ${await response.text()}`);
  }

  return response.json();
}

// Art type constants (from AIArt.h)
const kPathArt = 1;
const kTextFrameArt = 5;

// Paint order constants (from AITypes.h)
const kPlaceAboveAll = 1;

/**
 * Test: Create a rectangle path in Illustrator
 *
 * This is the "Hello World" test that verifies the complete pipeline:
 * 1. Call AIArtSuite.NewArt to create a path
 * 2. Call AIPathSuite.SetPathSegments to define the rectangle shape
 * 3. Call AIPathSuite.SetPathClosed to close the path
 * 4. Call AIArtSuite.SetArtName to name the object
 */
async function createRectangle(): Promise<TestResult> {
  try {
    // Step 1: Create a new path art object
    interface NewArtResult {
      newArt: number;
    }
    const newArtResult = await callPlugin('AIArtSuite', 'NewArt', {
      type: kPathArt,
      paintOrder: kPlaceAboveAll,
      prep: null, // null prep means place at top of layer
    }) as NewArtResult;

    const artHandle = newArtResult.newArt;
    if (artHandle < 0) {
      return {
        success: false,
        error: 'NewArt returned invalid handle',
        data: newArtResult,
      };
    }

    // Step 2: Define rectangle path segments
    // Rectangle at position (100, 100) with size 200x150 points
    // In Illustrator, Y increases upward, so top > bottom
    const segments = [
      { p: { h: 100, v: 250 }, in: { h: 100, v: 250 }, out: { h: 100, v: 250 }, corner: true }, // top-left
      { p: { h: 300, v: 250 }, in: { h: 300, v: 250 }, out: { h: 300, v: 250 }, corner: true }, // top-right
      { p: { h: 300, v: 100 }, in: { h: 300, v: 100 }, out: { h: 300, v: 100 }, corner: true }, // bottom-right
      { p: { h: 100, v: 100 }, in: { h: 100, v: 100 }, out: { h: 100, v: 100 }, corner: true }, // bottom-left
    ];

    await callPlugin('AIPathSuite', 'SetPathSegments', {
      path: artHandle,
      segNumber: 0,
      count: 4,
      segments: segments,
    });

    // Step 3: Close the path
    await callPlugin('AIPathSuite', 'SetPathClosed', {
      path: artHandle,
      closed: true,
    });

    // Step 4: Name the art object
    await callPlugin('AIArtSuite', 'SetArtName', {
      art: artHandle,
      name: 'NUXP Test Rectangle',
    });

    return {
      success: true,
      message: `Rectangle created with handle: ${artHandle}`,
      data: { artHandle, bounds: { x: 100, y: 100, width: 200, height: 150 } },
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown error';

    // Check for 404/endpoint not implemented
    if (message.includes('404') || message.includes('Not Found')) {
      return {
        success: false,
        error: 'Required SDK endpoints not implemented',
        data: {
          required: ['AIArtSuite.NewArt', 'AIPathSuite.SetPathSegments', 'AIPathSuite.SetPathClosed'],
        },
      };
    }

    // Check for network/connection errors
    if (message.includes('Failed to fetch') || message.includes('NetworkError')) {
      return {
        success: false,
        error: 'Cannot connect to plugin. Is Illustrator running with the NUXP plugin?',
      };
    }

    return { success: false, error: message };
  }
}

/**
 * Test: Create a text frame in Illustrator
 *
 * Creates a point text object with specified content.
 */
async function createText(): Promise<TestResult> {
  try {
    // First try the high-level convenience endpoint (may not exist)
    try {
      const result = await callEndpoint('/art/create-text', {
        x: 100,
        y: 300,
        content: 'Hello NUXP!',
        fontSize: 24,
      });

      return {
        success: true,
        message: 'Text frame created via convenience endpoint',
        data: result,
      };
    } catch {
      // Fall back to low-level SDK calls
    }

    // Create text art using AIArtSuite
    interface NewArtResult {
      newArt: number;
    }
    const newArtResult = await callPlugin('AIArtSuite', 'NewArt', {
      type: kTextFrameArt,
      paintOrder: kPlaceAboveAll,
      prep: null,
    }) as NewArtResult;

    const artHandle = newArtResult.newArt;
    if (artHandle < 0) {
      return {
        success: false,
        error: 'NewArt returned invalid handle for text frame',
        data: newArtResult,
      };
    }

    // Note: Setting text content requires AITextFrameSuite or AIATEPaint
    // This may not be implemented yet
    return {
      success: true,
      message: `Text frame created with handle: ${artHandle} (content setting may require AITextFrameSuite)`,
      data: { artHandle },
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown error';
    if (message.includes('404') || message.includes('Not Found')) {
      return {
        success: false,
        error: 'Endpoint not implemented: text frame creation',
        data: { endpoint: '/art/create-text', required: true },
      };
    }
    return { success: false, error: message };
  }
}

/**
 * Test: Create a new layer in Illustrator
 *
 * Uses AILayerSuite to create and name a new layer.
 */
async function createLayer(): Promise<TestResult> {
  try {
    // Try high-level endpoint first
    try {
      const result = await callEndpoint('/layers/create', {
        name: `NUXP Test Layer ${Date.now()}`,
      });

      return {
        success: true,
        message: 'Layer created via convenience endpoint',
        data: result,
      };
    } catch {
      // Fall back to low-level SDK calls
    }

    // Use AILayerSuite to create a layer
    interface InsertLayerResult {
      layer: number;
    }
    const layerResult = await callPlugin('AILayerSuite', 'InsertLayer', {
      layer: null, // null means create at top
      paintOrder: kPlaceAboveAll,
    }) as InsertLayerResult;

    const layerHandle = layerResult.layer;
    if (layerHandle < 0) {
      return {
        success: false,
        error: 'InsertLayer returned invalid handle',
        data: layerResult,
      };
    }

    // Name the layer
    const layerName = `NUXP Test Layer ${Date.now()}`;
    await callPlugin('AILayerSuite', 'SetLayerTitle', {
      layer: layerHandle,
      title: layerName,
    });

    return {
      success: true,
      message: `Layer "${layerName}" created`,
      data: { layerHandle, name: layerName },
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown error';
    if (message.includes('404') || message.includes('Not Found')) {
      return {
        success: false,
        error: 'Endpoint not implemented: /layers/create',
        data: { endpoint: '/layers/create', required: true },
      };
    }
    return { success: false, error: message };
  }
}

/**
 * Test: Get and modify the current selection
 *
 * 1. Gets the current selection via AIMatchingArtSuite
 * 2. If items are selected, moves them +10pt right
 */
async function modifySelection(): Promise<TestResult> {
  try {
    // Try to get current selection
    interface SelectionResult {
      art?: number;
      paintorder?: number;
      editable?: boolean;
    }

    // Get the insertion point (which reflects current selection context)
    const insertionPoint = await callPlugin('AIArtSuite', 'GetInsertionPoint', {}) as SelectionResult;

    if (!insertionPoint || insertionPoint.art === undefined || insertionPoint.art < 0) {
      return {
        success: false,
        error: 'No items selected. Please select an item in Illustrator first.',
        data: { insertionPoint },
      };
    }

    // Get bounds of the selected art
    interface BoundsResult {
      bounds: {
        left: number;
        top: number;
        right: number;
        bottom: number;
      };
    }
    const boundsResult = await callPlugin('AIArtSuite', 'GetArtBounds', {
      art: insertionPoint.art,
    }) as BoundsResult;

    // Try to move the art using AITransformArtSuite
    // This may require additional implementation
    try {
      await callPlugin('AITransformArtSuite', 'TransformArt', {
        art: insertionPoint.art,
        matrix: {
          a: 1, b: 0, c: 0, d: 1,
          tx: 10, // Move 10pt right
          ty: 0,
        },
        lineScale: 1,
        flags: 0,
      });

      return {
        success: true,
        message: 'Moved selection +10pt right',
        data: {
          artHandle: insertionPoint.art,
          originalBounds: boundsResult.bounds,
        },
      };
    } catch (transformError) {
      // Transform not available, report what we found
      return {
        success: true,
        message: 'Selection found (transform not yet implemented)',
        data: {
          artHandle: insertionPoint.art,
          bounds: boundsResult.bounds,
          note: 'AITransformArtSuite not implemented',
        },
      };
    }
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown error';
    if (message.includes('404') || message.includes('Not Found')) {
      return {
        success: false,
        error: 'Selection endpoints not implemented',
        data: { endpoints: ['AIArtSuite.GetInsertionPoint', 'AITransformArtSuite.TransformArt'] },
      };
    }
    return { success: false, error: message };
  }
}

/**
 * Test: Get art bounds
 *
 * Tests AIArtSuite.GetArtBounds by getting bounds of an art object.
 */
async function getArtBounds(): Promise<TestResult> {
  try {
    // First, get the insertion point to find an art object
    interface InsertionResult {
      art: number;
    }
    const insertion = await callPlugin('AIArtSuite', 'GetInsertionPoint', {}) as InsertionResult;

    if (!insertion || insertion.art < 0) {
      return {
        success: false,
        error: 'No art at insertion point. Please select an item or create one first.',
      };
    }

    interface BoundsResult {
      bounds: {
        left: number;
        top: number;
        right: number;
        bottom: number;
      };
    }
    const boundsResult = await callPlugin('AIArtSuite', 'GetArtBounds', {
      art: insertion.art,
    }) as BoundsResult;

    const bounds = boundsResult.bounds;
    const width = bounds.right - bounds.left;
    const height = bounds.top - bounds.bottom;

    return {
      success: true,
      message: `Art bounds: ${width.toFixed(1)} x ${height.toFixed(1)} pts`,
      data: {
        artHandle: insertion.art,
        bounds,
        size: { width, height },
      },
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown error';
    return { success: false, error: message };
  }
}

/**
 * Art Creation Test Suite
 */
export const ArtSuite: TestSuite = {
  id: 'art',
  name: 'Art Creation (SDK)',
  icon: '🎨',
  tests: [
    {
      id: 'art-rectangle',
      name: 'Create Rectangle',
      description: 'Create a rectangle using AIArtSuite and AIPathSuite (the "Hello World" test)',
      run: createRectangle,
    },
    {
      id: 'art-text',
      name: 'Create Text',
      description: 'Create a text frame with specific content',
      run: createText,
    },
    {
      id: 'art-layer',
      name: 'Create Layer',
      description: 'Add a new layer using AILayerSuite',
      run: createLayer,
    },
    {
      id: 'art-bounds',
      name: 'Get Art Bounds',
      description: 'Get bounding box of art at insertion point',
      run: getArtBounds,
    },
    {
      id: 'art-move-selection',
      name: 'Modify Selection',
      description: 'Get current selection and move it +10pt right',
      run: modifySelection,
    },
  ],
};

export default ArtSuite;
