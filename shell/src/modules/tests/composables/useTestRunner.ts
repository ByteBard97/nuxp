/**
 * useTestRunner Composable
 *
 * Manages test execution state and provides methods to run individual tests,
 * test suites, or all tests.
 */

import { ref, reactive } from 'vue';
import type { TestCase, TestResult, TestState, TestSuite } from '../types';

export function useTestRunner() {
  /** Map of test ID to current state */
  const testStates = reactive<Map<string, TestState>>(new Map());

  /** Whether any tests are currently running */
  const isRunning = ref(false);

  /** ID of the currently running test */
  const currentTest = ref<string | null>(null);

  /**
   * Get the current state of a test
   * @param testId - The test's unique identifier
   * @returns The test's current state, or pending if not yet run
   */
  function getState(testId: string): TestState {
    return testStates.get(testId) ?? { status: 'pending' };
  }

  /**
   * Run a single test case
   * @param test - The test case to run
   * @returns The test result
   */
  async function runTest(test: TestCase): Promise<TestResult> {
    currentTest.value = test.id;
    testStates.set(test.id, { status: 'running' });

    const startTime = performance.now();
    try {
      const result = await test.run();
      result.duration = performance.now() - startTime;
      testStates.set(test.id, {
        status: result.success ? 'passed' : 'failed',
        result,
      });
      return result;
    } catch (error) {
      const result: TestResult = {
        success: false,
        error: error instanceof Error ? error.message : String(error),
        duration: performance.now() - startTime,
      };
      testStates.set(test.id, { status: 'failed', result });
      return result;
    } finally {
      currentTest.value = null;
    }
  }

  /**
   * Run all tests in a suite sequentially
   * @param tests - Array of test cases to run
   */
  async function runSuite(tests: TestCase[]): Promise<void> {
    isRunning.value = true;
    for (const test of tests) {
      await runTest(test);
    }
    isRunning.value = false;
  }

  /**
   * Run all tests in all suites sequentially
   * @param suites - Array of test suites to run
   */
  async function runAll(suites: TestSuite[]): Promise<void> {
    isRunning.value = true;
    for (const suite of suites) {
      for (const test of suite.tests) {
        await runTest(test);
      }
    }
    isRunning.value = false;
  }

  /**
   * Reset all test states to pending
   */
  function reset(): void {
    testStates.clear();
    currentTest.value = null;
  }

  /**
   * Get summary statistics for all tests
   * @param suites - Array of test suites
   */
  function getStats(suites: TestSuite[]) {
    let total = 0;
    let passed = 0;
    let failed = 0;
    let pending = 0;
    let running = 0;

    for (const suite of suites) {
      for (const test of suite.tests) {
        total++;
        const state = getState(test.id);
        switch (state.status) {
          case 'passed':
            passed++;
            break;
          case 'failed':
            failed++;
            break;
          case 'running':
            running++;
            break;
          default:
            pending++;
        }
      }
    }

    return { total, passed, failed, pending, running };
  }

  return {
    testStates,
    isRunning,
    currentTest,
    getState,
    runTest,
    runSuite,
    runAll,
    reset,
    getStats,
  };
}
