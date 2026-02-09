<script setup lang="ts">
/**
 * DocumentStatus Component
 *
 * Displays information about the currently active Illustrator document
 * including name, dimensions, and layer count.
 */

import { computed, onMounted } from 'vue';
import { useConnectionStore } from '@/stores/connection';
import { useDocumentStore } from '@/stores/document';

// Stores
const connectionStore = useConnectionStore();
const documentStore = useDocumentStore();

// Computed
const documentInfo = computed(() => documentStore.currentDocument);

const statusClass = computed(() => {
  if (!connectionStore.connected) return 'disconnected';
  if (!documentStore.hasDocument) return 'no-document';
  return 'active';
});

const statusMessage = computed(() => {
  if (!connectionStore.connected) {
    return 'Connect to Illustrator to see document info';
  }
  if (!documentStore.hasDocument) {
    return 'No document is currently open in Illustrator';
  }
  return '';
});

// Methods
async function refreshDocument(): Promise<void> {
  if (connectionStore.connected) {
    await documentStore.refreshDocument();
  }
}

// Lifecycle
onMounted(() => {
  if (connectionStore.connected) {
    documentStore.refreshDocument();
  }
});
</script>

<template>
  <div class="document-status" :class="statusClass">
    <!-- Header -->
    <div class="status-header">
      <h3>Document Status</h3>
      <button
        class="refresh-btn"
        :disabled="!connectionStore.connected || documentStore.loading"
        @click="refreshDocument"
      >
        <span v-if="documentStore.loading" class="spinner"></span>
        <span v-else>Refresh</span>
      </button>
    </div>

    <!-- Status Message (when no document) -->
    <div v-if="statusMessage" class="status-message">
      <span class="status-icon">{{ connectionStore.connected ? '📄' : '🔌' }}</span>
      <span>{{ statusMessage }}</span>
    </div>

    <!-- Document Info (when document is open) -->
    <div v-else class="document-info">
      <!-- Document Name -->
      <div class="info-card name-card">
        <div class="card-icon">📄</div>
        <div class="card-content">
          <div class="card-label">Document Name</div>
          <div class="card-value">{{ documentInfo?.name || 'Untitled' }}</div>
        </div>
      </div>

      <!-- Dimensions -->
      <div class="info-card dimensions-card">
        <div class="card-icon">📐</div>
        <div class="card-content">
          <div class="card-label">Dimensions</div>
          <div class="card-value">{{ documentStore.dimensionsFormatted }}</div>
          <div v-if="documentInfo" class="card-detail">
            {{ documentInfo.width.toFixed(1) }} x {{ documentInfo.height.toFixed(1) }} points
          </div>
        </div>
      </div>

      <!-- Layer Count -->
      <div class="info-card layers-card">
        <div class="card-icon">📚</div>
        <div class="card-content">
          <div class="card-label">Layers</div>
          <div class="card-value">{{ documentStore.layerCount }}</div>
          <div class="card-detail">
            {{ documentStore.visibleLayers.length }} visible,
            {{ documentStore.lockedLayers.length }} locked
          </div>
        </div>
      </div>

      <!-- Selection Info -->
      <div class="info-card selection-card">
        <div class="card-icon">✏️</div>
        <div class="card-content">
          <div class="card-label">Selection</div>
          <div class="card-value">
            {{ documentStore.selectionCount }} item{{ documentStore.selectionCount !== 1 ? 's' : '' }}
          </div>
          <div v-if="documentStore.hasSelection" class="card-detail">
            {{ documentStore.selection.map(s => s.type).filter((v, i, a) => a.indexOf(v) === i).join(', ') }}
          </div>
        </div>
      </div>
    </div>

    <!-- Layer List -->
    <div v-if="documentStore.hasDocument && documentStore.layers.length > 0" class="layers-section">
      <h4>Layers</h4>
      <ul class="layer-list">
        <li
          v-for="layer in documentStore.layers"
          :key="layer.id"
          class="layer-item"
          :class="{ hidden: !layer.visible, locked: layer.locked }"
        >
          <span class="layer-visibility">{{ layer.visible ? '👁' : '👁‍🗨' }}</span>
          <span class="layer-lock">{{ layer.locked ? '🔒' : '' }}</span>
          <span class="layer-name">{{ layer.name }}</span>
        </li>
      </ul>
    </div>

    <!-- Last Refresh -->
    <div v-if="documentStore.lastRefresh" class="last-refresh">
      Last updated: {{ documentStore.lastRefresh.toLocaleTimeString() }}
    </div>
  </div>
</template>

<style scoped>
.document-status {
  padding: var(--spacing-lg);
  height: 100%;
  overflow-y: auto;
}

.status-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: var(--spacing-lg);
}

.status-header h3 {
  margin: 0;
}

.refresh-btn {
  display: flex;
  align-items: center;
  gap: var(--spacing-xs);
}

/* Status Message */
.status-message {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: var(--spacing-md);
  padding: var(--spacing-xl);
  background-color: var(--bg-secondary);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-md);
  text-align: center;
  color: var(--text-secondary);
}

.status-icon {
  font-size: 32px;
  opacity: 0.6;
}

/* Document Info Cards */
.document-info {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
  gap: var(--spacing-md);
}

.info-card {
  display: flex;
  gap: var(--spacing-md);
  padding: var(--spacing-md);
  background-color: var(--bg-secondary);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-md);
}

.card-icon {
  font-size: 24px;
  opacity: 0.8;
}

.card-content {
  flex: 1;
  min-width: 0;
}

.card-label {
  font-size: var(--font-size-xs);
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 0.5px;
  margin-bottom: var(--spacing-xs);
}

.card-value {
  font-size: var(--font-size-lg);
  font-weight: 500;
  color: var(--text-bright);
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

.card-detail {
  font-size: var(--font-size-xs);
  color: var(--text-muted);
  margin-top: var(--spacing-xs);
}

/* Layers Section */
.layers-section {
  margin-top: var(--spacing-lg);
}

.layers-section h4 {
  font-size: var(--font-size-sm);
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 0.5px;
  margin-bottom: var(--spacing-sm);
}

.layer-list {
  list-style: none;
  background-color: var(--bg-secondary);
  border: 1px solid var(--border-color);
  border-radius: var(--radius-md);
  overflow: hidden;
}

.layer-item {
  display: flex;
  align-items: center;
  gap: var(--spacing-sm);
  padding: var(--spacing-sm) var(--spacing-md);
  border-bottom: 1px solid var(--border-color);
  font-size: var(--font-size-sm);
}

.layer-item:last-child {
  border-bottom: none;
}

.layer-item.hidden {
  opacity: 0.5;
}

.layer-item.locked {
  background-color: rgba(255, 255, 255, 0.03);
}

.layer-visibility {
  width: 20px;
  font-size: var(--font-size-sm);
}

.layer-lock {
  width: 16px;
  font-size: var(--font-size-xs);
}

.layer-name {
  flex: 1;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

/* Last Refresh */
.last-refresh {
  margin-top: var(--spacing-lg);
  font-size: var(--font-size-xs);
  color: var(--text-muted);
  text-align: center;
}
</style>
