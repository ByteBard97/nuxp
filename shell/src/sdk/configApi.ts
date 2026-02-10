/**
 * Configuration API
 *
 * Functions for getting and setting plugin configuration.
 * Handles port changes with automatic SSE reconnection.
 */

import { getApiUrl, setPort, getPort } from './config';
import { sseClient } from './generated/events';

/**
 * Result of a port change operation
 */
export interface PortChangeResult {
  success: boolean;
  previousPort: number;
  newPort: number;
  message: string;
  error?: string;
}

/**
 * Server configuration from the plugin
 */
export interface ServerConfig {
  version: number;
  server: {
    port: number;
  };
}

/**
 * Get the current server configuration from the plugin
 *
 * @returns The server configuration
 */
export async function getServerConfig(): Promise<ServerConfig> {
  const response = await fetch(getApiUrl('/config'));

  if (!response.ok) {
    throw new Error(`Failed to get config: ${response.status} ${response.statusText}`);
  }

  const data = await response.json();

  if (!data.success) {
    throw new Error(data.error || 'Failed to get config');
  }

  return data.config;
}

/**
 * Change the server port
 *
 * This will:
 * 1. Send a request to the current port to change it
 * 2. Update the local config to use the new port
 * 3. Reconnect SSE to the new port
 *
 * @param newPort - The new port number (1024-65535)
 * @returns Result of the port change operation
 */
export async function changePort(newPort: number): Promise<PortChangeResult> {
  // Validate locally first
  if (newPort < 1024 || newPort > 65535) {
    return {
      success: false,
      previousPort: getPort(),
      newPort,
      message: '',
      error: 'Port must be between 1024 and 65535',
    };
  }

  const previousPort = getPort();

  // Don't change if it's the same port
  if (newPort === previousPort) {
    return {
      success: true,
      previousPort,
      newPort,
      message: 'Port unchanged',
    };
  }

  try {
    // Send request to current port
    const response = await fetch(getApiUrl('/config/port'), {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ port: newPort }),
    });

    const result = await response.json();

    if (result.success) {
      // Update local config
      setPort(newPort);

      // Reconnect SSE to new port after brief delay
      // (server needs time to restart)
      setTimeout(() => {
        sseClient.disconnect();
        sseClient.connect();
      }, 500);

      return {
        success: true,
        previousPort: result.previousPort,
        newPort: result.newPort,
        message: result.message,
      };
    } else {
      return {
        success: false,
        previousPort,
        newPort,
        message: '',
        error: result.error || 'Failed to change port',
      };
    }
  } catch (error) {
    return {
      success: false,
      previousPort,
      newPort,
      message: '',
      error: `Failed to change port: ${error instanceof Error ? error.message : String(error)}`,
    };
  }
}

/**
 * Discover the port where the NUXP plugin is running
 *
 * Scans common ports to find the plugin. Useful for initial connection
 * when the port is unknown.
 *
 * @param candidatePorts - Ports to try (default: [8080, 8081, 8082, 8083])
 * @returns The port where the plugin is running, or null if not found
 */
export async function discoverPort(
  candidatePorts: number[] = [8080, 8081, 8082, 8083]
): Promise<number | null> {
  for (const port of candidatePorts) {
    try {
      const controller = new AbortController();
      const timeoutId = setTimeout(() => controller.abort(), 500);

      const response = await fetch(`http://localhost:${port}/health`, {
        signal: controller.signal,
      });

      clearTimeout(timeoutId);

      if (response.ok) {
        const data = await response.json();
        // Verify it's NUXP, not some other service
        if (data.plugin === 'NUXP') {
          return port;
        }
      }
    } catch {
      // Port not responding, try next
    }
  }

  return null;
}

/**
 * Auto-connect to the plugin by discovering the port
 *
 * Attempts to discover the plugin port and configures the SDK to use it.
 *
 * @returns true if connected, false otherwise
 */
export async function autoConnect(): Promise<boolean> {
  const port = await discoverPort();

  if (port !== null) {
    setPort(port);
    return true;
  }

  return false;
}
