<script setup lang="ts">
/**
 * Sidebar Component
 *
 * Navigation sidebar with connection status indicator and navigation links.
 * Displays plugin connection status and provides navigation between views.
 */

import { computed } from 'vue';
import { useConnectionStore } from '@/stores/connection';
import { useDocumentStore } from '@/stores/document';

// Stores
const connectionStore = useConnectionStore();
const documentStore = useDocumentStore();

// Navigation items
interface NavItem {
  id: string;
  label: string;
  icon: string;
  badge?: number | null;
}

const navItems = computed<NavItem[]>(() => [
  {
    id: 'documents',
    label: 'Documents',
    icon: '📄',
    badge: documentStore.hasDocument ? 1 : null,
  },
  {
    id: 'layers',
    label: 'Layers',
    icon: '📚',
    badge: documentStore.layerCount > 0 ? documentStore.layerCount : null,
  },
  {
    id: 'selection',
    label: 'Selection',
    icon: '✏️',
    badge: documentStore.selectionCount > 0 ? documentStore.selectionCount : null,
  },
  {
    id: 'debug',
    label: 'Debug',
    icon: '🔧',
    badge: null,
  },
]);

// Active navigation item
const activeItem = defineModel<string>('activeItem', { default: 'debug' });

// Emits
const emit = defineEmits<{
  navigate: [item: string];
}>();

// Methods
function handleNavClick(itemId: string): void {
  activeItem.value = itemId;
  emit('navigate', itemId);
}

async function handleRefresh(): Promise<void> {
  await connectionStore.checkConnection();
  if (connectionStore.connected) {
    await documentStore.refreshAll();
  }
}
</script>

<template>
  <aside class="sidebar">
    <!-- Connection Status -->
    <div class="connection-status">
      <div class="status-indicator">
        <span
          class="status-dot"
          :class="connectionStore.connected ? 'connected' : 'disconnected'"
        ></span>
        <span class="status-text">
          {{ connectionStore.connected ? 'Connected' : 'Disconnected' }}
        </span>
      </div>
      <div v-if="connectionStore.pluginVersion" class="version-info">
        v{{ connectionStore.pluginVersion }}
      </div>
      <div v-if="connectionStore.error" class="error-info">
        {{ connectionStore.error }}
      </div>
    </div>

    <!-- Navigation -->
    <nav class="nav-list">
      <button
        v-for="item in navItems"
        :key="item.id"
        class="nav-item"
        :class="{ active: activeItem === item.id }"
        @click="handleNavClick(item.id)"
      >
        <span class="nav-icon">{{ item.icon }}</span>
        <span class="nav-label">{{ item.label }}</span>
        <span v-if="item.badge !== null" class="nav-badge">
          {{ item.badge }}
        </span>
      </button>
    </nav>

    <!-- Actions -->
    <div class="sidebar-actions">
      <button
        class="refresh-btn"
        :disabled="connectionStore.checking"
        @click="handleRefresh"
      >
        <span v-if="connectionStore.checking" class="spinner"></span>
        <span v-else>Refresh</span>
      </button>
      <div class="last-check">
        Last check: {{ connectionStore.lastCheckFormatted }}
      </div>
    </div>
  </aside>
</template>

<style scoped>
.sidebar {
  width: var(--sidebar-width);
  min-width: var(--sidebar-width);
  height: 100%;
  background-color: var(--bg-secondary);
  border-right: 1px solid var(--border-color);
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

/* Connection Status */
.connection-status {
  padding: var(--spacing-md);
  border-bottom: 1px solid var(--border-color);
}

.status-indicator {
  display: flex;
  align-items: center;
}

.status-text {
  font-size: var(--font-size-sm);
  font-weight: 500;
}

.version-info {
  margin-top: var(--spacing-xs);
  font-size: var(--font-size-xs);
  color: var(--text-secondary);
}

.error-info {
  margin-top: var(--spacing-xs);
  font-size: var(--font-size-xs);
  color: var(--accent-red);
  word-break: break-word;
}

/* Navigation */
.nav-list {
  flex: 1;
  padding: var(--spacing-sm);
  overflow-y: auto;
}

.nav-item {
  display: flex;
  align-items: center;
  width: 100%;
  padding: var(--spacing-sm) var(--spacing-md);
  margin-bottom: var(--spacing-xs);
  border: none;
  border-radius: var(--radius-md);
  background-color: transparent;
  color: var(--text-primary);
  cursor: pointer;
  transition: background-color var(--transition-fast);
  text-align: left;
}

.nav-item:hover {
  background-color: var(--bg-hover);
}

.nav-item.active {
  background-color: var(--bg-active);
  color: var(--text-bright);
}

.nav-icon {
  width: 24px;
  font-size: var(--font-size-md);
}

.nav-label {
  flex: 1;
  font-size: var(--font-size-sm);
}

.nav-badge {
  min-width: 20px;
  height: 20px;
  padding: 0 var(--spacing-xs);
  border-radius: 10px;
  background-color: var(--accent-blue);
  color: var(--text-bright);
  font-size: var(--font-size-xs);
  font-weight: 500;
  display: flex;
  align-items: center;
  justify-content: center;
}

/* Actions */
.sidebar-actions {
  padding: var(--spacing-md);
  border-top: 1px solid var(--border-color);
}

.refresh-btn {
  width: 100%;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: var(--spacing-sm);
}

.last-check {
  margin-top: var(--spacing-sm);
  font-size: var(--font-size-xs);
  color: var(--text-muted);
  text-align: center;
}
</style>
