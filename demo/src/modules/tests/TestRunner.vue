<script setup lang="ts">
/**
 * TestRunner Component
 *
 * Main test runner interface for the NUXP Test Plugin (QA App).
 * Displays test suites in a sidebar and test details in the main area.
 */

import { ref, computed } from 'vue';
import { Button, Panel } from '@/components/ui';
import { useTestRunner } from './composables/useTestRunner';
import type { TestSuite, TestStatus } from './types';

// Props
interface Props {
  suites: TestSuite[];
}

const props = defineProps<Props>();

// Test runner composable
const {
  isRunning,
  currentTest,
  getState,
  runTest,
  runSuite,
  runAll,
  reset,
  getStats,
} = useTestRunner();

// Currently selected suite
const selectedSuiteId = ref<string>(props.suites[0]?.id ?? '');

// Computed: currently selected suite object
const selectedSuite = computed(() => {
  return props.suites.find((s) => s.id === selectedSuiteId.value);
});

// Computed: overall stats
const stats = computed(() => getStats(props.suites));

// Expanded results tracking
const expandedResults = ref<Set<string>>(new Set());

// Toggle result expansion
function toggleResult(testId: string): void {
  if (expandedResults.value.has(testId)) {
    expandedResults.value.delete(testId);
  } else {
    expandedResults.value.add(testId);
  }
}

// Run all tests in the selected suite
async function handleRunSuite(): Promise<void> {
  if (selectedSuite.value) {
    await runSuite(selectedSuite.value.tests);
  }
}

// Run all tests in all suites
async function handleRunAll(): Promise<void> {
  await runAll(props.suites);
}

// Get status icon for a test
function getStatusIcon(status: TestStatus): string {
  switch (status) {
    case 'passed':
      return '✓';
    case 'failed':
      return '✗';
    case 'running':
      return '◌';
    case 'skipped':
      return '⊘';
    default:
      return '○';
  }
}

// Get suite status icon (based on its tests)
function getSuiteStatusIcon(suite: TestSuite): string {
  const states = suite.tests.map((t) => getState(t.id).status);
  if (states.every((s) => s === 'passed')) return '✓';
  if (states.some((s) => s === 'failed')) return '✗';
  if (states.some((s) => s === 'running')) return '◌';
  return '○';
}

// Format duration
function formatDuration(ms: number | undefined): string {
  if (ms === undefined) return '';
  if (ms < 1) return '<1ms';
  if (ms < 1000) return `${Math.round(ms)}ms`;
  return `${(ms / 1000).toFixed(2)}s`;
}
</script>

<template>
  <div class="test-runner">
    <!-- Header -->
    <header class="test-runner__header">
      <div class="test-runner__header-left">
        <h1 class="test-runner__title">Test Runner</h1>
        <span class="test-runner__subtitle">NUXP QA Test Suite</span>
      </div>
      <div class="test-runner__header-right">
        <div class="test-runner__stats">
          <span class="stat stat--total">{{ stats.total }} tests</span>
          <span v-if="stats.passed > 0" class="stat stat--passed">{{ stats.passed }} passed</span>
          <span v-if="stats.failed > 0" class="stat stat--failed">{{ stats.failed }} failed</span>
          <span v-if="stats.running > 0" class="stat stat--running">{{ stats.running }} running</span>
        </div>
        <div class="test-runner__actions">
          <Button variant="ghost" size="sm" :disabled="isRunning" @click="reset">
            Reset
          </Button>
          <Button variant="primary" size="sm" :loading="isRunning" @click="handleRunAll">
            Run All
          </Button>
        </div>
      </div>
    </header>

    <!-- Body -->
    <div class="test-runner__body">
      <!-- Sidebar: Suite List -->
      <aside class="test-runner__sidebar">
        <div class="suite-list">
          <button
            v-for="suite in props.suites"
            :key="suite.id"
            class="suite-item"
            :class="{ 'suite-item--active': suite.id === selectedSuiteId }"
            @click="selectedSuiteId = suite.id"
          >
            <span class="suite-item__icon">{{ suite.icon }}</span>
            <span class="suite-item__name">{{ suite.name }}</span>
            <span
              class="suite-item__status"
              :class="`suite-item__status--${getSuiteStatusIcon(suite) === '✓' ? 'passed' : getSuiteStatusIcon(suite) === '✗' ? 'failed' : getSuiteStatusIcon(suite) === '◌' ? 'running' : 'pending'}`"
            >
              {{ getSuiteStatusIcon(suite) }}
            </span>
          </button>
        </div>
      </aside>

      <!-- Main: Test List -->
      <main class="test-runner__main">
        <div v-if="selectedSuite" class="test-list">
          <!-- Suite Header -->
          <div class="test-list__header">
            <div class="test-list__header-info">
              <span class="test-list__header-icon">{{ selectedSuite.icon }}</span>
              <h2 class="test-list__header-title">{{ selectedSuite.name }}</h2>
              <span class="test-list__header-count">
                {{ selectedSuite.tests.length }} tests
              </span>
            </div>
            <Button
              variant="secondary"
              size="sm"
              :loading="isRunning"
              @click="handleRunSuite"
            >
              Run Suite
            </Button>
          </div>

          <!-- Test Items -->
          <div class="test-items">
            <div
              v-for="test in selectedSuite.tests"
              :key="test.id"
              class="test-item"
              :class="`test-item--${getState(test.id).status}`"
            >
              <div class="test-item__row">
                <span
                  class="test-item__status-icon"
                  :class="`test-item__status-icon--${getState(test.id).status}`"
                >
                  {{ getStatusIcon(getState(test.id).status) }}
                </span>
                <div class="test-item__info">
                  <span class="test-item__name">{{ test.name }}</span>
                  <span class="test-item__description">{{ test.description }}</span>
                </div>
                <div class="test-item__actions">
                  <span
                    v-if="getState(test.id).result?.duration !== undefined"
                    class="test-item__duration"
                  >
                    {{ formatDuration(getState(test.id).result?.duration) }}
                  </span>
                  <Button
                    variant="ghost"
                    size="sm"
                    :disabled="isRunning && currentTest !== test.id"
                    :loading="currentTest === test.id"
                    @click="runTest(test)"
                  >
                    Run
                  </Button>
                </div>
              </div>

              <!-- Result Panel (collapsible) -->
              <div
                v-if="getState(test.id).result"
                class="test-item__result"
              >
                <button
                  class="test-item__result-toggle"
                  @click="toggleResult(test.id)"
                >
                  <span class="toggle-icon">{{ expandedResults.has(test.id) ? '▼' : '▶' }}</span>
                  <span>{{ getState(test.id).result?.success ? 'Result' : 'Error Details' }}</span>
                </button>
                <div
                  v-if="expandedResults.has(test.id)"
                  class="test-item__result-content"
                >
                  <Panel variant="solid" padding="sm">
                    <div v-if="getState(test.id).result?.message" class="result-message">
                      {{ getState(test.id).result?.message }}
                    </div>
                    <div v-if="getState(test.id).result?.error" class="result-error">
                      {{ getState(test.id).result?.error }}
                    </div>
                    <pre v-if="getState(test.id).result?.data !== undefined" class="result-data">{{ JSON.stringify(getState(test.id).result?.data, null, 2) }}</pre>
                  </Panel>
                </div>
              </div>
            </div>
          </div>
        </div>

        <!-- Empty State -->
        <div v-else class="test-runner__empty">
          <p>Select a test suite to view tests</p>
        </div>
      </main>
    </div>
  </div>
</template>

<style scoped>
.test-runner {
  display: flex;
  flex-direction: column;
  height: 100%;
  background: var(--surface-1);
  color: var(--text-primary);
}

/* Header */
.test-runner__header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: var(--space-4) var(--space-6);
  background: var(--surface-2);
  border-bottom: 1px solid var(--border-subtle);
}

.test-runner__header-left {
  display: flex;
  align-items: baseline;
  gap: var(--space-3);
}

.test-runner__title {
  margin: 0;
  font-size: var(--text-xl);
  font-weight: var(--weight-semibold);
  color: var(--text-bright);
}

.test-runner__subtitle {
  font-size: var(--text-sm);
  color: var(--text-muted);
}

.test-runner__header-right {
  display: flex;
  align-items: center;
  gap: var(--space-6);
}

.test-runner__stats {
  display: flex;
  gap: var(--space-4);
}

.stat {
  font-size: var(--text-sm);
  padding: var(--space-1) var(--space-3);
  border-radius: var(--radius-sm);
  background: var(--surface-3);
}

.stat--passed {
  color: var(--color-success);
}

.stat--failed {
  color: var(--color-error);
}

.stat--running {
  color: var(--color-warning);
}

.test-runner__actions {
  display: flex;
  gap: var(--space-2);
}

/* Body Layout */
.test-runner__body {
  flex: 1;
  display: flex;
  overflow: hidden;
}

/* Sidebar */
.test-runner__sidebar {
  width: 240px;
  min-width: 240px;
  background: var(--surface-2);
  border-right: 1px solid var(--border-subtle);
  overflow-y: auto;
}

.suite-list {
  display: flex;
  flex-direction: column;
  padding: var(--space-2);
}

.suite-item {
  display: flex;
  align-items: center;
  gap: var(--space-3);
  padding: var(--space-3) var(--space-4);
  border: none;
  background: transparent;
  color: var(--text-secondary);
  font-size: var(--text-sm);
  cursor: pointer;
  border-radius: var(--radius-md);
  transition: all var(--transition-base);
  text-align: left;
}

.suite-item:hover {
  background: var(--surface-3);
  color: var(--text-primary);
}

.suite-item--active {
  background: var(--surface-4);
  color: var(--text-bright);
}

.suite-item__icon {
  font-size: var(--text-lg);
}

.suite-item__name {
  flex: 1;
}

.suite-item__status {
  font-size: var(--text-sm);
  font-weight: var(--weight-bold);
}

.suite-item__status--passed {
  color: var(--color-success);
}

.suite-item__status--failed {
  color: var(--color-error);
}

.suite-item__status--running {
  color: var(--color-warning);
  animation: pulse 1s infinite;
}

.suite-item__status--pending {
  color: var(--text-muted);
}

/* Main Content */
.test-runner__main {
  flex: 1;
  overflow-y: auto;
  padding: var(--space-4);
}

.test-list__header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  margin-bottom: var(--space-4);
  padding-bottom: var(--space-3);
  border-bottom: 1px solid var(--border-subtle);
}

.test-list__header-info {
  display: flex;
  align-items: center;
  gap: var(--space-3);
}

.test-list__header-icon {
  font-size: var(--text-2xl);
}

.test-list__header-title {
  margin: 0;
  font-size: var(--text-lg);
  font-weight: var(--weight-semibold);
  color: var(--text-bright);
}

.test-list__header-count {
  font-size: var(--text-sm);
  color: var(--text-muted);
  padding: var(--space-1) var(--space-2);
  background: var(--surface-3);
  border-radius: var(--radius-sm);
}

/* Test Items */
.test-items {
  display: flex;
  flex-direction: column;
  gap: var(--space-2);
}

.test-item {
  background: var(--surface-3);
  border-radius: var(--radius-md);
  border: 1px solid var(--border-subtle);
  overflow: hidden;
  transition: border-color var(--transition-base);
}

.test-item--passed {
  border-left: 3px solid var(--color-success);
}

.test-item--failed {
  border-left: 3px solid var(--color-error);
}

.test-item--running {
  border-left: 3px solid var(--color-warning);
}

.test-item__row {
  display: flex;
  align-items: center;
  gap: var(--space-3);
  padding: var(--space-3) var(--space-4);
}

.test-item__status-icon {
  width: 24px;
  height: 24px;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: var(--text-base);
  font-weight: var(--weight-bold);
  border-radius: 50%;
  background: var(--surface-4);
}

.test-item__status-icon--passed {
  color: var(--color-success);
  background: rgba(74, 222, 128, 0.15);
}

.test-item__status-icon--failed {
  color: var(--color-error);
  background: rgba(239, 68, 68, 0.15);
}

.test-item__status-icon--running {
  color: var(--color-warning);
  background: rgba(251, 191, 36, 0.15);
  animation: pulse 1s infinite;
}

.test-item__status-icon--pending,
.test-item__status-icon--skipped {
  color: var(--text-muted);
}

.test-item__info {
  flex: 1;
  display: flex;
  flex-direction: column;
  gap: var(--space-1);
  min-width: 0;
}

.test-item__name {
  font-size: var(--text-sm);
  font-weight: var(--weight-medium);
  color: var(--text-primary);
}

.test-item__description {
  font-size: var(--text-xs);
  color: var(--text-muted);
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

.test-item__actions {
  display: flex;
  align-items: center;
  gap: var(--space-3);
}

.test-item__duration {
  font-size: var(--text-xs);
  font-family: var(--font-mono);
  color: var(--text-muted);
}

/* Result Panel */
.test-item__result {
  border-top: 1px solid var(--border-subtle);
}

.test-item__result-toggle {
  display: flex;
  align-items: center;
  gap: var(--space-2);
  width: 100%;
  padding: var(--space-2) var(--space-4);
  border: none;
  background: var(--surface-4);
  color: var(--text-secondary);
  font-size: var(--text-xs);
  cursor: pointer;
  text-align: left;
  transition: background var(--transition-base);
}

.test-item__result-toggle:hover {
  background: var(--surface-5);
}

.toggle-icon {
  font-size: 10px;
}

.test-item__result-content {
  padding: var(--space-3);
  background: var(--surface-2);
}

.result-message {
  font-size: var(--text-sm);
  color: var(--text-primary);
  margin-bottom: var(--space-2);
}

.result-error {
  font-size: var(--text-sm);
  color: var(--color-error);
  margin-bottom: var(--space-2);
}

.result-data {
  margin: 0;
  padding: var(--space-3);
  background: var(--surface-1);
  border-radius: var(--radius-sm);
  font-family: var(--font-mono);
  font-size: var(--text-xs);
  color: var(--text-secondary);
  overflow-x: auto;
  white-space: pre-wrap;
  word-break: break-word;
}

/* Empty State */
.test-runner__empty {
  display: flex;
  align-items: center;
  justify-content: center;
  height: 100%;
  color: var(--text-muted);
}

/* Animations */
@keyframes pulse {
  0%, 100% {
    opacity: 1;
  }
  50% {
    opacity: 0.5;
  }
}
</style>
