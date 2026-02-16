<script setup lang="ts">
/**
 * App.vue - Main Application Component
 *
 * Root component that sets up the application layout with:
 * - Header with app title
 * - Sidebar for navigation
 * - Main content area for views
 * - Router support for standalone pages like /design
 */

import { ref, computed, onMounted, onUnmounted, watch } from 'vue';
import { useRoute } from 'vue-router';
import Sidebar from '@/components/Sidebar.vue';
import DebugPanel from '@/components/DebugPanel.vue';
import DocumentStatus from '@/components/DocumentStatus.vue';
import ScriptToolkit from '@/components/ScriptToolkit.vue';
import { useConnectionStore } from '@/stores/connection';
import { useDocumentStore } from '@/stores/document';
import { startEventLoop, stopEventLoop } from '@/sdk/bridge';
import { getPort } from '@/sdk/config';
import nuxpCrunch from '@/assets/nuxp-crunch.webp';
import nuxpIdle from '@/assets/nuxp-idle.webm';
import nuxpIcon from '@/assets/nuxp-icon.png';

const pluginPort = getPort();

// Router
const route = useRoute();

// Check if we're on a standalone route (like /design)
const isStandaloneRoute = computed(() => {
  return route.matched.length > 0;
});

// Stores
const connectionStore = useConnectionStore();
const documentStore = useDocumentStore();

// Current view state
const currentView = ref<string>('debug');

// Handle navigation from sidebar
function handleNavigate(viewId: string): void {
  currentView.value = viewId;
}

// Watch for connection changes to refresh document data
watch(
  () => connectionStore.connected,
  async (connected) => {
    if (connected) {
      await documentStore.refreshAll();
    } else {
      documentStore.reset();
    }
  }
);

// Lifecycle
onMounted(() => {
  // Start automatic health checks
  connectionStore.startAutoCheck();
  // Start event polling
  startEventLoop();
});

onUnmounted(() => {
  // Clean up
  connectionStore.stopAutoCheck();
  stopEventLoop();
});
</script>

<template>
  <!-- Standalone routes (like /design) render directly via router-view -->
  <router-view v-if="isStandaloneRoute" />

  <!-- Main application layout for the default view -->
  <div v-else class="app-container">
    <!-- Header -->
    <header class="app-header">
      <div class="header-title">
        <h1>NUXP</h1>
        <span class="header-subtitle">Illustrator SDK Toolkit</span>
      </div>
      <div class="header-status" :class="connectionStore.connected ? 'status-ok' : 'status-err'">
        <span
          class="status-dot"
          :class="connectionStore.connected ? 'connected' : 'disconnected'"
        ></span>
        <span class="status-label">
          {{ connectionStore.connected ? 'Connected' : 'Disconnected' }}
        </span>
      </div>
    </header>

    <!-- Main Layout -->
    <div class="app-body">
      <!-- Sidebar -->
      <Sidebar
        v-model:active-item="currentView"
        @navigate="handleNavigate"
      />

      <!-- Disconnected Splash -->
      <main v-if="!connectionStore.connected" class="app-content splash-container">
        <div class="splash">
          <video
            :src="nuxpIdle"
            autoplay
            loop
            muted
            playsinline
            class="splash-hero"
          >
            <img :src="nuxpCrunch" alt="NUXP Crunch" class="splash-hero" />
          </video>
          <div class="splash-text">
            <h2 class="splash-title">Waiting for Illustrator...</h2>
            <p class="splash-desc">
              Captain NUXP is standing by. Launch Illustrator with the
              NUXP plugin loaded, then open a document.
            </p>
            <div class="splash-steps">
              <div class="splash-step">
                <span class="step-num">1</span>
                <span>Build the plugin: <code>cmake --build build</code></span>
              </div>
              <div class="splash-step">
                <span class="step-num">2</span>
                <span>Install to Illustrator's Plug-ins folder</span>
              </div>
              <div class="splash-step">
                <span class="step-num">3</span>
                <span>Launch Illustrator and open a document</span>
              </div>
            </div>
            <div class="splash-pulse">
              <span class="pulse-dot"></span>
              <span class="pulse-label">Checking localhost:{{ pluginPort }}...</span>
            </div>
          </div>
        </div>
      </main>

      <!-- Connected Content Area -->
      <main v-else class="app-content">
        <DebugPanel v-if="currentView === 'debug'" />
        <DocumentStatus v-else-if="currentView === 'documents'" />
        <DocumentStatus v-else-if="currentView === 'layers'" />
        <DocumentStatus v-else-if="currentView === 'selection'" />
        <ScriptToolkit v-else-if="currentView === 'scripts'" />
        <div v-else class="placeholder-view">
          <img :src="nuxpIcon" alt="Captain NUXP" class="placeholder-icon" />
          <p>View not implemented: {{ currentView }}</p>
        </div>
      </main>
    </div>
  </div>
</template>

<style scoped>
.app-container {
  display: flex;
  flex-direction: column;
  height: 100%;
  width: 100%;
  overflow: hidden;
}

/* Header */
.app-header {
  height: var(--header-height);
  min-height: var(--header-height);
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 0 var(--spacing-lg);
  background-color: var(--bg-tertiary);
  border-bottom: 1px solid var(--border-color);
}

.header-title {
  display: flex;
  align-items: center;
  gap: var(--spacing-sm);
}

.header-title h1 {
  margin: 0;
  font-size: var(--font-size-xl);
  font-weight: 700;
  color: var(--text-bright);
  letter-spacing: 0.5px;
}

.header-subtitle {
  font-size: var(--font-size-sm);
  color: var(--text-secondary);
  margin-left: var(--spacing-xs);
}

.header-status {
  display: flex;
  align-items: center;
  gap: var(--spacing-sm);
  padding: var(--spacing-xs) var(--spacing-md);
  border-radius: var(--radius-md);
  transition: background-color 0.3s ease;
}

.header-status.status-ok {
  background-color: rgba(60, 154, 95, 0.15);
}

.header-status.status-err {
  background-color: rgba(231, 76, 60, 0.15);
}

.status-label {
  font-size: var(--font-size-sm);
  color: var(--text-secondary);
}

/* Body Layout */
.app-body {
  flex: 1;
  display: flex;
  overflow: hidden;
}

/* Content Area */
.app-content {
  flex: 1;
  overflow: hidden;
  background-color: var(--bg-primary);
}

/* Disconnected Splash */
.splash-container {
  display: flex;
  align-items: center;
  justify-content: center;
  overflow-y: auto;
}

.splash {
  display: flex;
  align-items: center;
  gap: 48px;
  padding: 48px;
  max-width: 800px;
}

.splash-hero {
  width: 240px;
  height: auto;
  border-radius: 12px;
  box-shadow: 0 8px 32px rgba(0, 0, 0, 0.5);
  flex-shrink: 0;
  animation: float 3s ease-in-out infinite;
}

@keyframes float {
  0%, 100% { transform: translateY(0px); }
  50% { transform: translateY(-8px); }
}

.splash-text {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.splash-title {
  font-size: 24px;
  font-weight: 700;
  color: var(--text-bright);
  margin: 0;
}

.splash-desc {
  font-size: 14px;
  color: var(--text-secondary);
  line-height: 1.6;
  margin: 0;
}

.splash-steps {
  display: flex;
  flex-direction: column;
  gap: 8px;
  margin-top: 8px;
}

.splash-step {
  display: flex;
  align-items: center;
  gap: 12px;
  font-size: 13px;
  color: var(--text-primary);
}

.splash-step code {
  background-color: var(--bg-tertiary);
  padding: 2px 6px;
  border-radius: 3px;
  font-size: 12px;
  color: var(--accent-green);
}

.step-num {
  width: 24px;
  height: 24px;
  border-radius: 50%;
  background: linear-gradient(135deg, var(--accent-blue), #0066cc);
  color: white;
  font-size: 12px;
  font-weight: 700;
  display: flex;
  align-items: center;
  justify-content: center;
  flex-shrink: 0;
}

.splash-pulse {
  display: flex;
  align-items: center;
  gap: 8px;
  margin-top: 12px;
  padding: 8px 12px;
  background-color: rgba(231, 76, 60, 0.1);
  border: 1px solid rgba(231, 76, 60, 0.2);
  border-radius: 6px;
  width: fit-content;
}

.pulse-dot {
  width: 8px;
  height: 8px;
  border-radius: 50%;
  background-color: var(--accent-red);
  animation: pulse 1.5s ease-in-out infinite;
}

@keyframes pulse {
  0%, 100% { opacity: 1; transform: scale(1); }
  50% { opacity: 0.4; transform: scale(0.8); }
}

.pulse-label {
  font-size: 12px;
  color: var(--text-secondary);
  font-family: 'SF Mono', Monaco, 'Courier New', monospace;
}

/* Placeholder */
.placeholder-view {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 16px;
  height: 100%;
  color: var(--text-muted);
}

.placeholder-icon {
  width: 64px;
  height: 64px;
  border-radius: 12px;
  opacity: 0.4;
}
</style>
