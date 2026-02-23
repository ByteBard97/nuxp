/**
 * Async Error Handling Utilities
 *
 * Provides safe wrappers for async operations that might fail silently.
 * All errors are caught and surfaced to the user via browser alerts.
 *
 * Design principle: FAIL LOUDLY -- no silent failures.
 */

/**
 * Show an error message to the user via browser alert.
 *
 * @param message - Primary error message
 * @param context - Optional context describing where the error occurred
 */
export async function showError(message: string, context?: string): Promise<void> {
  console.error('[FAIL LOUDLY]', message, context)
  alert(`Error: ${message}${context ? `\n\nContext: ${context}` : ''}`)
}

/**
 * Show a warning message to the user via browser alert.
 *
 * @param message - Primary warning message
 * @param context - Optional context describing the situation
 */
export async function showWarning(message: string, context?: string): Promise<void> {
  console.warn('[FAIL LOUDLY]', message, context)
  alert(`Warning: ${message}${context ? `\n\nContext: ${context}` : ''}`)
}

/**
 * Show a confirmation dialog via browser confirm.
 *
 * @param message - Confirmation prompt text
 * @param context - Optional additional context
 * @returns true if user clicked OK, false if user clicked Cancel
 */
export async function showConfirm(message: string, context?: string): Promise<boolean> {
  return confirm(`${message}${context ? `\n\n${context}` : ''}`)
}

/**
 * Wrap setInterval with error catching.
 * Errors in the async callback are caught and displayed to the user.
 *
 * @param asyncFn - Async function to call on each interval
 * @param ms - Interval in milliseconds
 * @param context - Context string for error messages
 * @returns Interval ID (can be passed to clearInterval)
 */
export function safeSetInterval(
  asyncFn: () => Promise<void>,
  ms: number,
  context: string
): ReturnType<typeof setInterval> {
  return setInterval(() => {
    asyncFn().catch(async (err) => {
      console.error(`[safeSetInterval] Error in ${context}:`, err)
      await showError(
        err?.message || String(err),
        `During scheduled operation: ${context}`
      )
    })
  }, ms)
}

/**
 * Wrap setTimeout with error catching.
 * Errors in the async callback are caught and displayed to the user.
 *
 * @param asyncFn - Async function to call after timeout
 * @param ms - Timeout in milliseconds
 * @param context - Context string for error messages
 * @returns Timeout ID (can be passed to clearTimeout)
 */
export function safeSetTimeout(
  asyncFn: () => Promise<void>,
  ms: number,
  context: string
): ReturnType<typeof setTimeout> {
  return setTimeout(() => {
    asyncFn().catch(async (err) => {
      console.error(`[safeSetTimeout] Error in ${context}:`, err)
      await showError(
        err?.message || String(err),
        `During delayed operation: ${context}`
      )
    })
  }, ms)
}

/**
 * Wrap an async event handler with error catching.
 * Use this for addEventListener callbacks and similar fire-and-forget handlers.
 *
 * @param asyncFn - Async function to wrap
 * @param context - Context string for error messages
 * @returns Wrapped function that catches and displays errors
 */
export function wrapAsyncHandler<T extends (...args: any[]) => Promise<void>>(
  asyncFn: T,
  context: string
): (...args: Parameters<T>) => void {
  return (...args: Parameters<T>) => {
    asyncFn(...args).catch(async (err) => {
      console.error(`[wrapAsyncHandler] Error in ${context}:`, err)
      await showError(
        err?.message || String(err),
        `During event handler: ${context}`
      )
    })
  }
}

/**
 * Safely execute a fire-and-forget async operation.
 * Catches errors and optionally displays them to the user.
 *
 * @param asyncFn - Async function to execute
 * @param context - Context string for error messages
 * @param options - Configuration options
 */
export async function safeExecute(
  asyncFn: () => Promise<void>,
  context: string,
  options: {
    /** Whether to show error to user (default: true) */
    showToUser?: boolean
    /** Whether to log to console (default: true) */
    logToConsole?: boolean
  } = {}
): Promise<void> {
  const { showToUser = true, logToConsole = true } = options

  try {
    await asyncFn()
  } catch (err: any) {
    if (logToConsole) {
      console.error(`[safeExecute] Error in ${context}:`, err)
    }
    if (showToUser) {
      await showError(
        err?.message || String(err),
        context
      )
    }
  }
}

/**
 * Create a debounced async function with error catching.
 * Unlike regular debounce, this will not leave promises hanging on error.
 *
 * @param asyncFn - Async function to debounce
 * @param ms - Debounce delay in milliseconds
 * @param context - Context string for error messages
 * @returns Debounced function
 */
export function createDebouncedAsync<T extends (...args: any[]) => Promise<any>>(
  asyncFn: T,
  ms: number,
  context: string
): (...args: Parameters<T>) => Promise<ReturnType<T> | undefined> {
  let timeoutId: ReturnType<typeof setTimeout> | null = null
  let currentResolve: ((value: ReturnType<T> | undefined) => void) | null = null

  return (...args: Parameters<T>): Promise<ReturnType<T> | undefined> => {
    if (timeoutId) {
      clearTimeout(timeoutId)
      if (currentResolve) {
        currentResolve(undefined)
      }
    }

    return new Promise((resolve, reject) => {
      currentResolve = resolve

      timeoutId = setTimeout(async () => {
        timeoutId = null
        try {
          const result = await asyncFn(...args)
          resolve(result)
        } catch (err: any) {
          console.error(`[createDebouncedAsync] Error in ${context}:`, err)
          await showError(
            err?.message || String(err),
            `During debounced operation: ${context}`
          )
          reject(err)
        } finally {
          currentResolve = null
        }
      }, ms)
    })
  }
}

/**
 * Tracks consecutive failures for operations that retry.
 * Shows a warning to the user after the failure threshold is exceeded.
 */
export class FailureTracker {
  private failures = 0
  private readonly threshold: number
  private readonly context: string

  /**
   * @param threshold - Number of consecutive failures before alerting the user
   * @param context - Description of the tracked operation
   */
  constructor(threshold: number, context: string) {
    this.threshold = threshold
    this.context = context
  }

  /**
   * Record a failure. Returns true if the threshold was exceeded.
   *
   * @param error - The error that occurred
   * @returns true if the failure count has reached the threshold
   */
  async recordFailure(error: Error | string): Promise<boolean> {
    this.failures++
    console.warn(`[FailureTracker] ${this.context}: Failure ${this.failures}/${this.threshold}`, error)

    if (this.failures >= this.threshold) {
      await showWarning(
        `${this.context} has failed ${this.failures} times`,
        typeof error === 'string' ? error : error.message
      )
      return true
    }
    return false
  }

  /** Reset the failure count (call on success). */
  reset(): void {
    this.failures = 0
  }

  /** Get the current failure count. */
  get count(): number {
    return this.failures
  }
}
