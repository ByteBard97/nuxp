<script setup lang="ts">
/**
 * DebugPanel Component
 *
 * Debug information panel showing connection status, plugin version,
 * last API response, and controls for testing the plugin connection.
 */

import { ref, computed } from 'vue';
import { useConnectionStore } from '@/stores/connection';
import { useDocumentStore } from '@/stores/document';
import { getPluginUrl, getHealthInfo } from '@/services/api';
import { getActiveDocument, getSelection } from '@/services/illustrator';

// Stores
const connectionStore = useConnectionStore();
const documentStore = useDocumentStore();

// Local state
const testingHealth = ref(false);
const testingDocument = ref(false);
const testingSelection = ref(false);
const testResult = ref<{ success: boolean; message: string } | null>(null);

// Computed
const pluginUrl = computed(() => getPluginUrl());

const lastResponseJson = computed(() => {
  if (documentStore.lastApiResponse === null) {
    return 'No API calls made yet';
  }
  try {
    return JSON.stringify(documentStore.lastApiResponse, null, 2);
  } catch {
    return String(documentStore.lastApiResponse);
  }
});

// Methods
async function testHealthEndpoint(): Promise<void> {
  testingHealth.value = true;
  testResult.value = null;

  try {
    const healthInfo = await getHealthInfo();
    if (healthInfo) {
      testResult.value = {
        success: true,
        message: `Health OK - Version: ${healthInfo.version}`,
      };
      documentStore.setLastApiResponse(healthInfo);
    } else {
      testResult.value = {
        success: false,
        message: 'Health check failed - no response',
      };
    }
  } catch (error) {
    testResult.value = {
      success: false,
      message: `Error: ${error instanceof Error ? error.message : String(error)}`,
    };
  } finally {
    testingHealth.value = false;
  }
}

async function testDocumentEndpoint(): Promise<void> {
  testingDocument.value = true;
  testResult.value = null;

  try {
    const doc = await getActiveDocument();
    if (doc) {
      testResult.value = {
        success: true,
        message: `Document: ${doc.name} (${doc.width} x ${doc.height})`,
      };
      documentStore.setLastApiResponse(doc);
    } else {
      testResult.value = {
        success: false,
        message: 'No document open or plugin not responding',
      };
      documentStore.setLastApiResponse({ document: null });
    }
  } catch (error) {
    testResult.value = {
      success: false,
      message: `Error: ${error instanceof Error ? error.message : String(error)}`,
    };
  } finally {
    testingDocument.value = false;
  }
}

async function testSelectionEndpoint(): Promise<void> {
  testingSelection.value = true;
  testResult.value = null;

  try {
    const selection = await getSelection();
    testResult.value = {
      success: true,
      message: `Selection: ${selection.length} item(s)`,
    };
    documentStore.setLastApiResponse({ selection });
  } catch (error) {
    testResult.value = {
      success: false,
      message: `Error: ${error instanceof Error ? error.message : String(error)}`,
    };
  } finally {
    testingSelection.value = false;
  }
}

function clearResult(): void {
  testResult.value = null;
}
</script>

<template>
  <div class="debug-panel">
    <h2>Debug Panel</h2>

    <!-- Connection Info -->
    <div class="panel">
      <div class="panel-header">Connection Status</div>
      <div class="panel-content">
        <div class="info-row">
          <span class="label">Status</span>
          <span class="value">
            <span
              class="status-dot"
              :class="connectionStore.connected ? 'connected' : 'disconnected'"
            ></span>
            {{ connectionStore.statusText }}
          </span>
        </div>
        <div class="info-row">
          <span class="label">Plugin URL</span>
          <span class="value">{{ pluginUrl }}</span>
        </div>
        <div class="info-row">
          <span class="label">Plugin Version</span>
          <span class="value">
            {{ connectionStore.pluginVersion || 'Unknown' }}
          </span>
        </div>
        <div class="info-row">
          <span class="label">Last Check</span>
          <span class="value">{{ connectionStore.lastCheckFormatted }}</span>
        </div>
        <div v-if="connectionStore.error" class="info-row error">
          <span class="label">Error</span>
          <span class="value error-text">{{ connectionStore.error }}</span>
        </div>
      </div>
    </div>

    <!-- Test Endpoints -->
    <div class="panel mt-md">
      <div class="panel-header">Test Endpoints</div>
      <div class="panel-content">
        <div class="button-group">
          <button
            :disabled="testingHealth"
            @click="testHealthEndpoint"
          >
            <span v-if="testingHealth" class="spinner"></span>
            <span v-else>Test Health</span>
          </button>
          <button
            :disabled="testingDocument"
            @click="testDocumentEndpoint"
          >
            <span v-if="testingDocument" class="spinner"></span>
            <span v-else>Test Document</span>
          </button>
          <button
            :disabled="testingSelection"
            @click="testSelectionEndpoint"
          >
            <span v-if="testingSelection" class="spinner"></span>
            <span v-else>Test Selection</span>
          </button>
        </div>

        <!-- Test Result -->
        <div
          v-if="testResult"
          class="test-result mt-md"
          :class="{ success: testResult.success, error: !testResult.success }"
        >
          <span class="result-icon">{{ testResult.success ? '✓' : '✗' }}</span>
          <span class="result-message">{{ testResult.message }}</span>
          <button class="clear-btn" @click="clearResult">×</button>
        </div>
      </div>
    </div>

    <!-- Last API Response -->
    <div class="panel mt-md">
      <div class="panel-header">Last API Response</div>
      <div class="panel-content">
        <pre class="response-json">{{ lastResponseJson }}</pre>
      </div>
    </div>
  </div>
</template>

<style scoped>
.debug-panel {
  padding: var(--spacing-lg);
  overflow-y: auto;
  height: 100%;
}

.debug-panel h2 {
  margin-bottom: var(--spacing-lg);
}

.button-group {
  display: flex;
  gap: var(--spacing-sm);
  flex-wrap: wrap;
}

.button-group button {
  flex: 1;
  min-width: 100px;
  display: flex;
  align-items: center;
  justify-content: center;
  gap: var(--spacing-xs);
}

.test-result {
  display: flex;
  align-items: center;
  gap: var(--spacing-sm);
  padding: var(--spacing-sm) var(--spacing-md);
  border-radius: var(--radius-md);
  font-size: var(--font-size-sm);
}

.test-result.success {
  background-color: rgba(60, 154, 95, 0.2);
  border: 1px solid var(--accent-green);
}

.test-result.error {
  background-color: rgba(231, 76, 60, 0.2);
  border: 1px solid var(--accent-red);
}

.result-icon {
  font-weight: bold;
}

.test-result.success .result-icon {
  color: var(--accent-green);
}

.test-result.error .result-icon {
  color: var(--accent-red);
}

.result-message {
  flex: 1;
}

.clear-btn {
  width: 24px;
  height: 24px;
  padding: 0;
  border: none;
  background: transparent;
  color: var(--text-secondary);
  cursor: pointer;
  font-size: var(--font-size-lg);
  line-height: 1;
}

.clear-btn:hover {
  color: var(--text-primary);
  background: transparent;
}

.response-json {
  max-height: 300px;
  overflow: auto;
  white-space: pre-wrap;
  word-break: break-word;
}

.info-row.error {
  margin-top: var(--spacing-sm);
  padding-top: var(--spacing-sm);
  border-top: 1px solid var(--border-color);
}

.error-text {
  color: var(--accent-red);
}
</style>
