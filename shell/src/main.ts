/**
 * Main entry point for NUXP Shell
 *
 * Initializes Vue application with Pinia store and mounts to DOM.
 */

import { createApp } from 'vue';
import { pinia } from '@/stores';
import App from '@/App.vue';
import '@/styles/main.css';

// Create Vue application
const app = createApp(App);

// Install Pinia store
app.use(pinia);

// Mount application
app.mount('#app');
