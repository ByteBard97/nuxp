/**
 * System & Connectivity Test Suite
 *
 * Tests for verifying plugin connectivity, health, and system information.
 * These are fundamental tests that should pass before running any other suites.
 */

import type { TestSuite, TestResult } from '../types';

const PLUGIN_URL = 'http://localhost:8080';

/**
 * Health response structure from the C++ plugin
 */
interface HealthData {
  status: string;
  version?: string;
  message?: string;
}

/**
 * Test: Health Check
 *
 * Verifies the C++ plugin HTTP server is responding at /health endpoint.
 */
async function healthCheck(): Promise<TestResult> {
  try {
    const response = await fetch(`${PLUGIN_URL}/health`);
    if (!response.ok) {
      return {
        success: false,
        error: `HTTP ${response.status}: ${response.statusText}`,
      };
    }

    const data = (await response.json()) as HealthData;
    return {
      success: true,
      message: `Plugin is healthy (status: ${data.status})`,
      data,
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown error';

    // Check for network/connection errors
    if (message.includes('Failed to fetch') || message.includes('NetworkError')) {
      return {
        success: false,
        error: `Cannot connect to plugin at ${PLUGIN_URL}. Is Illustrator running with the NUXP plugin?`,
      };
    }

    return {
      success: false,
      error: message,
    };
  }
}

/**
 * Test: Round Trip Latency
 *
 * Measures average response time over multiple requests to the health endpoint.
 * This helps diagnose performance issues with the plugin communication.
 */
async function roundTripLatency(): Promise<TestResult> {
  const iterations = 10;
  const times: number[] = [];

  try {
    for (let i = 0; i < iterations; i++) {
      const start = performance.now();
      const response = await fetch(`${PLUGIN_URL}/health`);
      const elapsed = performance.now() - start;

      if (!response.ok) {
        return {
          success: false,
          error: `Request ${i + 1} failed: HTTP ${response.status}`,
          data: { completedIterations: i, times },
        };
      }

      times.push(elapsed);
    }

    const avg = times.reduce((a, b) => a + b, 0) / times.length;
    const min = Math.min(...times);
    const max = Math.max(...times);

    return {
      success: true,
      message: `Avg: ${avg.toFixed(2)}ms (min: ${min.toFixed(2)}ms, max: ${max.toFixed(2)}ms)`,
      data: {
        iterations,
        avg: Number(avg.toFixed(2)),
        min: Number(min.toFixed(2)),
        max: Number(max.toFixed(2)),
        times: times.map((t) => Number(t.toFixed(2))),
      },
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown error';

    if (message.includes('Failed to fetch') || message.includes('NetworkError')) {
      return {
        success: false,
        error: 'Connection lost during latency test. Is the plugin still running?',
        data: { completedIterations: times.length, times },
      };
    }

    return {
      success: false,
      error: message,
      data: { completedIterations: times.length, times },
    };
  }
}

/**
 * Test: Version Info
 *
 * Retrieves and displays version information from:
 * - C++ plugin (from /health endpoint)
 * - Tauri (if running in Tauri context)
 * - Shell frontend (from package.json)
 */
async function versionInfo(): Promise<TestResult> {
  const versions: Record<string, string> = {
    plugin: 'Unknown',
    tauri: 'N/A',
    shell: '0.1.0', // From package.json
  };

  try {
    // Get plugin version from health endpoint
    try {
      const response = await fetch(`${PLUGIN_URL}/health`);
      if (response.ok) {
        const data = (await response.json()) as HealthData;
        versions.plugin = data.version ?? 'Connected (version not reported)';
      } else {
        versions.plugin = `Unavailable (HTTP ${response.status})`;
      }
    } catch {
      versions.plugin = 'Unavailable (connection failed)';
    }

    // Get Tauri version if available
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    if (typeof window !== 'undefined' && (window as any).__TAURI__) {
      try {
        const { getVersion } = await import('@tauri-apps/api/app');
        versions.tauri = await getVersion();
      } catch {
        versions.tauri = 'Available but version unavailable';
      }
    }

    return {
      success: true,
      message: `Plugin: ${versions.plugin}, Shell: ${versions.shell}`,
      data: versions,
    };
  } catch (error) {
    return {
      success: false,
      error: error instanceof Error ? error.message : 'Version check failed',
      data: versions,
    };
  }
}

/**
 * Test: API Endpoint Check
 *
 * Verifies the main /api/call endpoint is available and responding.
 * This is the primary endpoint used for all SDK calls.
 */
async function apiEndpointCheck(): Promise<TestResult> {
  try {
    // Send a minimal test request to /api/call
    // Using a non-existent suite to test endpoint availability without side effects
    const response = await fetch(`${PLUGIN_URL}/api/call`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
        suite: 'TestSuite',
        method: 'Ping',
        args: {},
      }),
    });

    // Even if the suite doesn't exist, a 400/404 means the endpoint is responding
    // A 200 means it worked, which is also good
    if (response.ok) {
      const data = await response.json();
      return {
        success: true,
        message: '/api/call endpoint is responding',
        data: { status: response.status, response: data },
      };
    }

    // 404 might mean the endpoint exists but the suite/method doesn't
    if (response.status === 404) {
      const errorText = await response.text();
      // Check if it's "suite not found" vs "endpoint not found"
      if (errorText.includes('suite') || errorText.includes('method')) {
        return {
          success: true,
          message: '/api/call endpoint is responding (test method not implemented)',
          data: { status: response.status, note: errorText },
        };
      }
    }

    // 400 typically means bad request format but endpoint exists
    if (response.status === 400) {
      return {
        success: true,
        message: '/api/call endpoint is responding',
        data: { status: response.status, note: 'Endpoint active, test request rejected' },
      };
    }

    return {
      success: false,
      error: `Unexpected response: HTTP ${response.status}`,
      data: { status: response.status, body: await response.text() },
    };
  } catch (error) {
    const message = error instanceof Error ? error.message : 'Unknown error';

    if (message.includes('Failed to fetch') || message.includes('NetworkError')) {
      return {
        success: false,
        error: 'Cannot connect to /api/call endpoint',
      };
    }

    return {
      success: false,
      error: message,
    };
  }
}

/**
 * System & Connectivity Test Suite
 */
export const SystemSuite: TestSuite = {
  id: 'system',
  name: 'System & Connectivity',
  icon: '🔌',
  tests: [
    {
      id: 'system-health',
      name: 'Health Check',
      description: 'Verify the C++ plugin HTTP server is responding',
      run: healthCheck,
    },
    {
      id: 'system-latency',
      name: 'Round Trip Latency',
      description: 'Measure average response time over 10 requests',
      run: roundTripLatency,
    },
    {
      id: 'system-api',
      name: 'API Endpoint Check',
      description: 'Verify /api/call endpoint is available for SDK calls',
      run: apiEndpointCheck,
    },
    {
      id: 'system-version',
      name: 'Version Info',
      description: 'Display plugin, Tauri, and shell versions',
      run: versionInfo,
    },
  ],
};

export default SystemSuite;
