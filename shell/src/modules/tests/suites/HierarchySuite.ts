import type { TestSuite, TestResult } from '../types';

const PLUGIN_URL = 'http://localhost:8080';

async function callPlugin(endpoint: string, data?: unknown): Promise<any> {
  const response = await fetch(`${PLUGIN_URL}${endpoint}`, {
    method: data ? 'POST' : 'GET',
    headers: data ? { 'Content-Type': 'application/json' } : {},
    body: data ? JSON.stringify(data) : undefined
  });

  if (!response.ok) {
    throw new Error(`HTTP ${response.status}: ${await response.text()}`);
  }

  return response.json();
}

async function countItems(): Promise<TestResult> {
  try {
    const result = await callPlugin('/document/count-items');

    return {
      success: true,
      message: `Document contains ${result.total} items`,
      data: result
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown error';
    if (message.includes('404') || message.includes('Not Found')) {
      return {
        success: false,
        error: 'Endpoint not implemented: /document/count-items',
        data: {
          endpoint: '/document/count-items',
          description: 'Should return recursive count of all art items in document'
        }
      };
    }
    return { success: false, error: message };
  }
}

async function getSelectionDetails(): Promise<TestResult> {
  try {
    const result = await callPlugin('/selection/details');

    if (!result || !result.items || result.items.length === 0) {
      return {
        success: true,
        message: 'No items selected',
        data: { items: [], count: 0 }
      };
    }

    return {
      success: true,
      message: `${result.items.length} item(s) selected`,
      data: result
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown error';
    if (message.includes('404') || message.includes('Not Found')) {
      return {
        success: false,
        error: 'Endpoint not implemented: /selection/details',
        data: {
          endpoint: '/selection/details',
          description: 'Should return detailed JSON of selected items (bounds, type, name, etc.)'
        }
      };
    }
    return { success: false, error: message };
  }
}

async function listLayers(): Promise<TestResult> {
  try {
    const result = await callPlugin('/layers/list');

    return {
      success: true,
      message: `Document has ${result.layers?.length ?? 0} layers`,
      data: result
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown error';
    if (message.includes('404') || message.includes('Not Found')) {
      return {
        success: false,
        error: 'Endpoint not implemented: /layers/list',
        data: { endpoint: '/layers/list' }
      };
    }
    return { success: false, error: message };
  }
}

async function getDocumentInfo(): Promise<TestResult> {
  try {
    const result = await callPlugin('/document/info');

    return {
      success: true,
      message: result.name ? `Document: ${result.name}` : 'Document info retrieved',
      data: result
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown error';
    if (message.includes('404') || message.includes('Not Found')) {
      return {
        success: false,
        error: 'Endpoint not implemented: /document/info',
        data: {
          endpoint: '/document/info',
          description: 'Should return document name, dimensions, color mode, etc.'
        }
      };
    }
    return { success: false, error: message };
  }
}

export const HierarchySuite: TestSuite = {
  id: 'hierarchy',
  name: 'Hierarchy & Traversal',
  icon: '🌳',
  tests: [
    {
      id: 'hierarchy-document',
      name: 'Document Info',
      description: 'Get basic document properties (name, size, color mode)',
      run: getDocumentInfo
    },
    {
      id: 'hierarchy-layers',
      name: 'List Layers',
      description: 'Enumerate all layers in the document',
      run: listLayers
    },
    {
      id: 'hierarchy-count',
      name: 'Count Items',
      description: 'Recursive count of all art items in the document',
      run: countItems
    },
    {
      id: 'hierarchy-selection',
      name: 'Selection Details',
      description: 'Detailed JSON dump of selected items (bounds, type, name)',
      run: getSelectionDetails
    }
  ]
};

export default HierarchySuite;
