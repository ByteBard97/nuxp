/**
 * Main entry point for NUXP Demo
 *
 * Initializes the SDK bridge, Vue application with Pinia store,
 * Vue Router, and mounts to DOM.
 */

import { createApp } from 'vue';
import { createBridge, setBridgeInstance } from '@nuxp/sdk';
import { pinia } from '@/stores';
import router from '@/router';
import App from '@/App.vue';
import '@/styles/main.css';
import { NUXP_PORT } from '@/config';

// ─── SDK Bridge Initialization ──────────────────────────────────────────────
// The bridge must be created and registered before any generated suite
// functions (CountLayers, GetSelection, etc.) are called.

const bridge = createBridge({ port: NUXP_PORT });
setBridgeInstance(bridge);

// ─── Vue Application ────────────────────────────────────────────────────────

const app = createApp(App);

app.use(pinia);
app.use(router);

app.mount('#app');
