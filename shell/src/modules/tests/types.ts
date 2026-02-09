/**
 * Type definitions for the NUXP Test Plugin system
 */

/**
 * Possible status values for a test
 */
export type TestStatus = 'pending' | 'running' | 'passed' | 'failed' | 'skipped';

/**
 * Result of running a single test
 */
export interface TestResult {
  /** Whether the test passed */
  success: boolean;
  /** Human-readable message describing the result */
  message?: string;
  /** Error message if the test failed */
  error?: string;
  /** Additional data returned by the test */
  data?: unknown;
  /** Duration of the test in milliseconds (set by runner) */
  duration?: number;
}

/**
 * A single test case within a suite
 */
export interface TestCase {
  /** Unique identifier for this test */
  id: string;
  /** Display name for the test */
  name: string;
  /** Description of what the test validates */
  description: string;
  /** Function that runs the test and returns a result */
  run: () => Promise<TestResult>;
}

/**
 * A collection of related tests
 */
export interface TestSuite {
  /** Unique identifier for this suite */
  id: string;
  /** Display name for the suite */
  name: string;
  /** Icon to display (emoji or icon class) */
  icon: string;
  /** Array of test cases in this suite */
  tests: TestCase[];
}

/**
 * Current state of a test execution
 */
export interface TestState {
  /** Current status of the test */
  status: TestStatus;
  /** Result if the test has completed */
  result?: TestResult;
}
