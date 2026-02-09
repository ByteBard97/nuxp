<script setup lang="ts">
/**
 * DebugPanel Component
 *
 * Debug information panel showing connection status, plugin version,
 * last API response, and controls for testing the plugin connection.
 */

import { ref, computed, onMounted, onUnmounted } from 'vue';
import { useConnectionStore } from '@/stores/connection';
import { useDocumentStore } from '@/stores/document';
import { getPluginUrl, getHealthInfo } from '@/services/api';
import { getActiveDocument, getSelection } from '@/services/illustrator';
import { callCpp, BridgeError, events, startEventLoop, stopEventLoop } from '@/sdk';
import { writeFile } from '@/services/filesystem';

// Art type constants (from AIArt.h)
const kPathArt = 1;

// Paint order constants (from AITypes.h)
const kPlaceAboveAll = 1;

// Stores
const connectionStore = useConnectionStore();
const documentStore = useDocumentStore();

// Local state
const testingHealth = ref(false);
const testingDocument = ref(false);
const testingSelection = ref(false);
const creatingRectangle = ref(false);
const savingLog = ref(false);
const isTauri = ref(false);
const testResult = ref<{ success: boolean; message: string } | null>(null);

// Event log state
interface EventLogEntry {
  type: string;
  time: string;
  data?: unknown;
}
const recentEvents = ref<EventLogEntry[]>([]);
const eventLoopActive = ref(false);
let eventUnsubscribe: (() => void) | null = null;

// Check if running in Tauri and start event loop
onMounted(() => {
  isTauri.value = !!(window as unknown as { __TAURI__?: unknown }).__TAURI__;

  // Start event loop and subscribe to all events
  startEventLoop();
  eventLoopActive.value = true;

  eventUnsubscribe = events.on('*', (event: { type: string; data?: unknown }) => {
    recentEvents.value.unshift({
      type: event.type,
      time: new Date().toLocaleTimeString(),
      data: event.data,
    });
    // Keep only last 10 events
    if (recentEvents.value.length > 10) {
      recentEvents.value.pop();
    }
  });
});

// Clean up on unmount
onUnmounted(() => {
  stopEventLoop();
  eventLoopActive.value = false;
  if (eventUnsubscribe) {
    eventUnsubscribe();
    eventUnsubscribe = null;
  }
});

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

/**
 * Clear the event log
 */
function clearEventLog(): void {
  recentEvents.value = [];
}

/**
 * Toggle the event polling loop
 */
function toggleEventLoop(): void {
  if (eventLoopActive.value) {
    stopEventLoop();
    eventLoopActive.value = false;
  } else {
    startEventLoop();
    eventLoopActive.value = true;
  }
}

/**
 * Save current debug log to filesystem
 *
 * Writes the current connection status and last API response
 * to a debug.log file in the app's data directory.
 */
async function saveLogToFile(): Promise<void> {
  if (!isTauri.value) {
    testResult.value = {
      success: false,
      message: 'Save Log requires Tauri (not running in desktop app)',
    };
    return;
  }

  savingLog.value = true;
  testResult.value = null;

  try {
    const timestamp = new Date().toISOString();
    const logContent = [
      `=== NUXP Debug Log ===`,
      `Timestamp: ${timestamp}`,
      ``,
      `--- Connection Status ---`,
      `Status: ${connectionStore.statusText}`,
      `Connected: ${connectionStore.connected}`,
      `Plugin URL: ${pluginUrl.value}`,
      `Plugin Version: ${connectionStore.pluginVersion || 'Unknown'}`,
      `Last Check: ${connectionStore.lastCheckFormatted}`,
      connectionStore.error ? `Error: ${connectionStore.error}` : '',
      ``,
      `--- Last API Response ---`,
      lastResponseJson.value,
      ``,
      `=== End of Log ===`,
    ].filter(Boolean).join('\n');

    await writeFile('debug.log', logContent);

    testResult.value = {
      success: true,
      message: `Log saved to debug.log at ${timestamp}`,
    };
  } catch (error) {
    console.error('[DebugPanel] Failed to save log:', error);
    testResult.value = {
      success: false,
      message: `Failed to save log: ${error instanceof Error ? error.message : String(error)}`,
    };
  } finally {
    savingLog.value = false;
  }
}

/**
 * Create a test rectangle in Illustrator
 *
 * This demonstrates the end-to-end pipeline by:
 * 1. Creating a new path art object
 * 2. Setting its name to "Hello NUXP"
 * 3. Setting its path segments to form a 100x100 rectangle
 */
async function createTestRectangle(): Promise<void> {
  creatingRectangle.value = true;
  testResult.value = null;

  try {
    // Step 1: Create a new path art object
    // NewArt(type, paintOrder, prep) -> returns { newArt: handle }
    const newArtResult = await callCpp<{ newArt: number }>(
      'AIArtSuite',
      'NewArt',
      {
        type: kPathArt,
        paintOrder: kPlaceAboveAll,
        prep: 0, // null prep means place at top of layer
      }
    );

    const artHandle = newArtResult.newArt;
    console.log('[DebugPanel] Created new art with handle:', artHandle);

    // Step 2: Set the art name to "Hello NUXP"
    await callCpp('AIArtSuite', 'SetArtName', {
      art: artHandle,
      name: 'Hello NUXP',
    });
    console.log('[DebugPanel] Set art name to "Hello NUXP"');

    // Step 3: Set path segments to form a 100x100 rectangle
    // Rectangle at position (100, 100) with size 100x100 points
    // In Illustrator, Y increases upward, so top > bottom
    const segments = [
      { p: { h: 100, v: 200 }, in: { h: 100, v: 200 }, out: { h: 100, v: 200 }, corner: true }, // top-left
      { p: { h: 200, v: 200 }, in: { h: 200, v: 200 }, out: { h: 200, v: 200 }, corner: true }, // top-right
      { p: { h: 200, v: 100 }, in: { h: 200, v: 100 }, out: { h: 200, v: 100 }, corner: true }, // bottom-right
      { p: { h: 100, v: 100 }, in: { h: 100, v: 100 }, out: { h: 100, v: 100 }, corner: true }, // bottom-left
    ];

    await callCpp('AIPathSuite', 'SetPathSegments', {
      path: artHandle,
      segNumber: 0,
      count: 4,
      segments: segments,
    });
    console.log('[DebugPanel] Set path segments for rectangle');

    // Step 4: Close the path to complete the rectangle
    await callCpp('AIPathSuite', 'SetPathClosed', {
      path: artHandle,
      closed: true,
    });
    console.log('[DebugPanel] Closed the path');

    // Success! Update the result
    testResult.value = {
      success: true,
      message: `Created rectangle "Hello NUXP" (handle: ${artHandle})`,
    };

    // Store the API response for display
    documentStore.setLastApiResponse({
      action: 'createTestRectangle',
      artHandle,
      name: 'Hello NUXP',
      bounds: { left: 100, top: 200, right: 200, bottom: 100 },
    });

  } catch (error) {
    console.error('[DebugPanel] Failed to create rectangle:', error);

    let message = 'Failed to create rectangle';
    if (error instanceof BridgeError) {
      message = `${error.message} (${error.suite}.${error.method})`;
    } else if (error instanceof Error) {
      message = error.message;
    }

    testResult.value = {
      success: false,
      message,
    };
  } finally {
    creatingRectangle.value = false;
  }
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

        <!-- Create Art Section -->
        <div class="section-divider"></div>
        <div class="section-header">Create Art</div>
        <div class="button-group">
          <button
            class="create-btn"
            :disabled="creatingRectangle"
            @click="createTestRectangle"
          >
            <span v-if="creatingRectangle" class="spinner"></span>
            <span v-else>Create Test Rectangle</span>
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

    <!-- Event Log -->
    <div class="panel mt-md">
      <div class="panel-header">
        <span>
          Event Log
          <span
            class="event-status-dot"
            :class="eventLoopActive ? 'active' : 'inactive'"
            :title="eventLoopActive ? 'Event loop running' : 'Event loop stopped'"
          ></span>
        </span>
        <div class="header-buttons">
          <button
            class="toggle-btn"
            :class="{ active: eventLoopActive }"
            @click="toggleEventLoop"
            :title="eventLoopActive ? 'Stop event loop' : 'Start event loop'"
          >
            {{ eventLoopActive ? 'Stop' : 'Start' }}
          </button>
          <button
            class="clear-events-btn"
            @click="clearEventLog"
            :disabled="recentEvents.length === 0"
            title="Clear event log"
          >
            Clear
          </button>
        </div>
      </div>
      <div class="panel-content">
        <div v-if="recentEvents.length === 0" class="no-events">
          No events received yet
        </div>
        <div v-else class="event-list">
          <div
            v-for="(event, index) in recentEvents"
            :key="index"
            class="event-item"
          >
            <span class="event-time">{{ event.time }}</span>
            <span class="event-type">{{ event.type }}</span>
            <span v-if="event.data" class="event-data">
              {{ typeof event.data === 'object' ? JSON.stringify(event.data) : event.data }}
            </span>
          </div>
        </div>
      </div>
    </div>

    <!-- Last API Response -->
    <div class="panel mt-md">
      <div class="panel-header">
        <span>Last API Response</span>
        <button
          class="save-log-btn"
          :disabled="savingLog || !isTauri"
          :title="isTauri ? 'Save debug log to file' : 'Save Log requires Tauri desktop app'"
          @click="saveLogToFile"
        >
          <span v-if="savingLog" class="spinner"></span>
          <span v-else>Save Log</span>
        </button>
      </div>
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

.section-divider {
  margin: var(--spacing-md) 0;
  border-top: 1px solid var(--border-color);
}

.section-header {
  font-size: var(--font-size-sm);
  font-weight: 600;
  color: var(--text-secondary);
  margin-bottom: var(--spacing-sm);
  text-transform: uppercase;
  letter-spacing: 0.5px;
}

.create-btn {
  background-color: var(--accent-blue);
  color: white;
}

.create-btn:hover:not(:disabled) {
  background-color: var(--accent-blue-hover, #2980b9);
  filter: brightness(1.1);
}

.create-btn:disabled {
  opacity: 0.6;
  cursor: not-allowed;
}

/* Panel header with button support */
.panel-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.save-log-btn {
  font-size: var(--font-size-xs);
  padding: var(--spacing-xs) var(--spacing-sm);
  min-width: auto;
  background-color: var(--bg-tertiary);
  border: 1px solid var(--border-color);
  display: inline-flex;
  align-items: center;
  gap: var(--spacing-xs);
}

.save-log-btn:hover:not(:disabled) {
  background-color: var(--bg-secondary);
  border-color: var(--accent-blue);
}

.save-log-btn:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

/* Event Log Styles */
.event-status-dot {
  display: inline-block;
  width: 8px;
  height: 8px;
  border-radius: 50%;
  margin-left: var(--spacing-xs);
  vertical-align: middle;
}

.event-status-dot.active {
  background-color: var(--accent-green);
  box-shadow: 0 0 4px var(--accent-green);
}

.event-status-dot.inactive {
  background-color: var(--text-secondary);
}

.header-buttons {
  display: flex;
  gap: var(--spacing-xs);
}

.toggle-btn,
.clear-events-btn {
  font-size: var(--font-size-xs);
  padding: var(--spacing-xs) var(--spacing-sm);
  min-width: auto;
  background-color: var(--bg-tertiary);
  border: 1px solid var(--border-color);
}

.toggle-btn:hover:not(:disabled),
.clear-events-btn:hover:not(:disabled) {
  background-color: var(--bg-secondary);
  border-color: var(--accent-blue);
}

.toggle-btn.active {
  background-color: rgba(60, 154, 95, 0.2);
  border-color: var(--accent-green);
}

.clear-events-btn:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.no-events {
  color: var(--text-secondary);
  font-style: italic;
  text-align: center;
  padding: var(--spacing-md);
}

.event-list {
  max-height: 200px;
  overflow-y: auto;
}

.event-item {
  display: flex;
  gap: var(--spacing-sm);
  padding: var(--spacing-xs) 0;
  border-bottom: 1px solid var(--border-color);
  font-size: var(--font-size-sm);
  align-items: flex-start;
}

.event-item:last-child {
  border-bottom: none;
}

.event-time {
  color: var(--text-secondary);
  flex-shrink: 0;
  font-family: monospace;
  font-size: var(--font-size-xs);
}

.event-type {
  color: var(--accent-blue);
  font-weight: 600;
  flex-shrink: 0;
}

.event-data {
  color: var(--text-primary);
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
  font-family: monospace;
  font-size: var(--font-size-xs);
}
</style>
