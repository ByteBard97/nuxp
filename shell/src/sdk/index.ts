/**
 * NUXP SDK
 *
 * TypeScript SDK for communicating with the C++ Illustrator plugin.
 *
 * @example
 * ```typescript
 * import { callCpp, isPluginAvailable } from '@/sdk';
 *
 * // Check if plugin is running
 * if (await isPluginAvailable()) {
 *   // Call a plugin method
 *   const name = await callCpp<string>('AIArt', 'GetArtName', { art: handle });
 * }
 * ```
 */

// Main bridge exports
export {
  callCpp,
  callCppBatch,
  isPluginAvailable,
  startEventLoop,
  stopEventLoop,
  BridgeError,
  type CppResponse,
} from './bridge';

// Event system exports
export {
  events,
  eventBus,
  type EventType,
  type EventCallback,
  type PluginEvent,
} from './events';

// Configuration exports
export {
  sdkConfig,
  updateSdkConfig,
  getApiUrl,
  type SdkConfig,
  DEFAULT_PORT,
  DEFAULT_HOST,
  DEFAULT_TIMEOUT_MS,
} from './config';

// Mock utilities (for testing and development)
export {
  isMockModeEnabled,
  registerMockHandler,
  clearMockHandlers,
} from './mockBridge';
