/**
 * Pinia store setup
 *
 * This module configures and exports the Pinia store instance
 * for use throughout the application.
 */

import { createPinia } from 'pinia';

/**
 * Create and configure the Pinia store
 */
export const pinia = createPinia();

// Re-export stores for convenience
export { useConnectionStore } from './connection';
export { useDocumentStore } from './document';
