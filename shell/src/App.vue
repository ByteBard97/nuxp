<script setup lang="ts">
/**
 * App.vue - Main Application Component
 *
 * Root component that sets up the application layout with:
 * - Header with app title
 * - Sidebar for navigation
 * - Main content area for views
 */

import { ref, onMounted, onUnmounted, watch } from 'vue';
import Sidebar from '@/components/Sidebar.vue';
import DebugPanel from '@/components/DebugPanel.vue';
import DocumentStatus from '@/components/DocumentStatus.vue';
import { useConnectionStore } from '@/stores/connection';
import { useDocumentStore } from '@/stores/document';

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
});

onUnmounted(() => {
  // Clean up
  connectionStore.stopAutoCheck();
});
</script>

<template>
  <div class="app-container">
    <!-- Header -->
    <header class="app-header">
      <div class="header-title">
        <h1>NUXP Debug</h1>
        <span class="header-subtitle">Illustrator Plugin Shell</span>
      </div>
      <div class="header-status">
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

      <!-- Content Area -->
      <main class="app-content">
        <DebugPanel v-if="currentView === 'debug'" />
        <DocumentStatus v-else-if="currentView === 'documents'" />
        <DocumentStatus v-else-if="currentView === 'layers'" />
        <DocumentStatus v-else-if="currentView === 'selection'" />
        <div v-else class="placeholder-view">
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
  align-items: baseline;
  gap: var(--spacing-md);
}

.header-title h1 {
  margin: 0;
  font-size: var(--font-size-lg);
  font-weight: 600;
  color: var(--text-bright);
}

.header-subtitle {
  font-size: var(--font-size-sm);
  color: var(--text-secondary);
}

.header-status {
  display: flex;
  align-items: center;
  gap: var(--spacing-sm);
  padding: var(--spacing-xs) var(--spacing-md);
  background-color: var(--bg-secondary);
  border-radius: var(--radius-md);
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

.placeholder-view {
  display: flex;
  align-items: center;
  justify-content: center;
  height: 100%;
  color: var(--text-muted);
}
</style>
