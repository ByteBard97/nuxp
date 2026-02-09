/**
 * Main entry point for NUXP Shell
 *
 * Initializes Vue application with Pinia store, Vue Router, and mounts to DOM.
 */

import { createApp } from 'vue';
import { pinia } from '@/stores';
import router from '@/router';
import App from '@/App.vue';
import '@/styles/main.css';

// Create Vue application
const app = createApp(App);

// Install Pinia store
app.use(pinia);

// Install Vue Router
app.use(router);

// Mount application
app.mount('#app');
