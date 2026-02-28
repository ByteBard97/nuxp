import { describe, it, expect, vi, beforeEach } from 'vitest'
import {
  showError,
  showWarning,
  showConfirm,
  wrapAsyncHandler,
  safeExecute,
  createDebouncedAsync,
  FailureTracker,
} from './asyncErrorHandling'

// Mock browser globals
beforeEach(() => {
  vi.stubGlobal('alert', vi.fn())
  vi.stubGlobal('confirm', vi.fn(() => true))
  vi.spyOn(console, 'error').mockImplementation(() => {})
  vi.spyOn(console, 'warn').mockImplementation(() => {})
})

// ---------------------------------------------------------------------------
// FailureTracker
// ---------------------------------------------------------------------------

describe('FailureTracker', () => {
  it('starts with a count of zero', () => {
    const tracker = new FailureTracker(3, 'test')
    expect(tracker.count).toBe(0)
  })

  it('increments count on each recordFailure call', async () => {
    const tracker = new FailureTracker(5, 'test')
    await tracker.recordFailure('err')
    expect(tracker.count).toBe(1)
    await tracker.recordFailure('err')
    expect(tracker.count).toBe(2)
  })

  it('returns false when below threshold', async () => {
    const tracker = new FailureTracker(3, 'test')
    expect(await tracker.recordFailure('err')).toBe(false)
    expect(await tracker.recordFailure('err')).toBe(false)
  })

  it('returns true when threshold is reached', async () => {
    const tracker = new FailureTracker(3, 'test')
    await tracker.recordFailure('err')
    await tracker.recordFailure('err')
    const hit = await tracker.recordFailure('err')
    expect(hit).toBe(true)
  })

  it('returns true when threshold is exceeded', async () => {
    const tracker = new FailureTracker(2, 'test')
    await tracker.recordFailure('err')
    await tracker.recordFailure('err')
    const exceeded = await tracker.recordFailure('err')
    expect(exceeded).toBe(true)
  })

  it('resets count to zero', async () => {
    const tracker = new FailureTracker(3, 'test')
    await tracker.recordFailure('err')
    await tracker.recordFailure('err')
    tracker.reset()
    expect(tracker.count).toBe(0)
  })

  it('accepts Error objects', async () => {
    const tracker = new FailureTracker(1, 'test')
    const hit = await tracker.recordFailure(new Error('boom'))
    expect(hit).toBe(true)
  })
})

// ---------------------------------------------------------------------------
// wrapAsyncHandler
// ---------------------------------------------------------------------------

describe('wrapAsyncHandler', () => {
  it('calls the wrapped function with the same arguments', async () => {
    const fn = vi.fn(async (_a: number, _b: string) => {})
    const wrapped = wrapAsyncHandler(fn, 'test')
    wrapped(42, 'hello')
    // Let microtask queue flush
    await vi.waitFor(() => expect(fn).toHaveBeenCalledWith(42, 'hello'))
  })

  it('does not throw when the async function throws', () => {
    const fn = vi.fn(async () => { throw new Error('boom') })
    const wrapped = wrapAsyncHandler(fn, 'test')
    // This should not throw synchronously
    expect(() => wrapped()).not.toThrow()
  })

  it('calls showError (via alert) when the async function throws', async () => {
    const fn = vi.fn(async () => { throw new Error('async boom') })
    const wrapped = wrapAsyncHandler(fn, 'test')
    wrapped()
    // Wait for the error handling promise chain to settle
    await vi.waitFor(() => {
      expect(alert).toHaveBeenCalled()
    })
  })
})

// ---------------------------------------------------------------------------
// createDebouncedAsync
// ---------------------------------------------------------------------------

describe('createDebouncedAsync', () => {
  beforeEach(() => {
    vi.useFakeTimers()
  })

  it('debounces multiple calls so only the last fires', async () => {
    const fn = vi.fn(async (val: number) => val)
    const debounced = createDebouncedAsync(fn, 100, 'test')

    const p1 = debounced(1)
    const p2 = debounced(2)
    const p3 = debounced(3)

    // Earlier promises resolve to undefined (cancelled)
    expect(await p1).toBeUndefined()
    expect(await p2).toBeUndefined()

    // Advance timers to fire the debounced call
    vi.advanceTimersByTime(100)

    expect(await p3).toBe(3)
    expect(fn).toHaveBeenCalledTimes(1)
    expect(fn).toHaveBeenCalledWith(3)

    vi.useRealTimers()
  })

  it('fires immediately if only one call is made', async () => {
    const fn = vi.fn(async () => 'result')
    const debounced = createDebouncedAsync(fn, 50, 'test')

    const promise = debounced()
    vi.advanceTimersByTime(50)

    expect(await promise).toBe('result')
    expect(fn).toHaveBeenCalledTimes(1)

    vi.useRealTimers()
  })
})

// ---------------------------------------------------------------------------
// safeExecute
// ---------------------------------------------------------------------------

describe('safeExecute', () => {
  it('executes the function without error', async () => {
    const fn = vi.fn(async () => {})
    await safeExecute(fn, 'test')
    expect(fn).toHaveBeenCalled()
  })

  it('catches errors without throwing', async () => {
    const fn = async () => { throw new Error('fail') }
    // Should not throw
    await safeExecute(fn, 'test')
  })

  it('calls showError (via alert) when the function throws and showToUser is true', async () => {
    const fn = async () => { throw new Error('visible error') }
    await safeExecute(fn, 'testContext', { showToUser: true })
    expect(alert).toHaveBeenCalled()
    const alertCall = (alert as ReturnType<typeof vi.fn>).mock.calls[0][0] as string
    expect(alertCall).toContain('visible error')
  })

  it('does not call alert when showToUser is false', async () => {
    const fn = async () => { throw new Error('hidden error') }
    await safeExecute(fn, 'test', { showToUser: false })
    expect(alert).not.toHaveBeenCalled()
  })

  it('logs to console by default', async () => {
    const fn = async () => { throw new Error('logged error') }
    await safeExecute(fn, 'test')
    expect(console.error).toHaveBeenCalled()
  })

  it('does not log when logToConsole is false', async () => {
    const fn = async () => { throw new Error('silent error') }
    await safeExecute(fn, 'test', { logToConsole: false })
    // console.error may still be called by showError, so check the safeExecute-specific log
    // The showError call itself logs, but the safeExecute guard does not
    // We verify by checking that showToUser:false + logToConsole:false means no console.error from safeExecute
    ;(console.error as ReturnType<typeof vi.fn>).mockClear()
    await safeExecute(fn, 'test', { logToConsole: false, showToUser: false })
    expect(console.error).not.toHaveBeenCalled()
  })
})

// ---------------------------------------------------------------------------
// showError / showWarning / showConfirm
// ---------------------------------------------------------------------------

describe('showError', () => {
  it('calls alert with the error message', async () => {
    await showError('Something failed')
    expect(alert).toHaveBeenCalledWith('Error: Something failed')
  })

  it('includes context in the alert when provided', async () => {
    await showError('Something failed', 'during init')
    const msg = (alert as ReturnType<typeof vi.fn>).mock.calls[0][0] as string
    expect(msg).toContain('Something failed')
    expect(msg).toContain('during init')
  })

  it('logs to console.error', async () => {
    await showError('test error')
    expect(console.error).toHaveBeenCalled()
  })
})

describe('showWarning', () => {
  it('calls alert with the warning message', async () => {
    await showWarning('Watch out')
    expect(alert).toHaveBeenCalledWith('Warning: Watch out')
  })

  it('includes context when provided', async () => {
    await showWarning('Watch out', 'in module X')
    const msg = (alert as ReturnType<typeof vi.fn>).mock.calls[0][0] as string
    expect(msg).toContain('Watch out')
    expect(msg).toContain('in module X')
  })
})

describe('showConfirm', () => {
  it('returns true when user confirms', async () => {
    ;(confirm as ReturnType<typeof vi.fn>).mockReturnValue(true)
    const result = await showConfirm('Are you sure?')
    expect(result).toBe(true)
  })

  it('returns false when user cancels', async () => {
    ;(confirm as ReturnType<typeof vi.fn>).mockReturnValue(false)
    const result = await showConfirm('Are you sure?')
    expect(result).toBe(false)
  })

  it('includes context in the confirm message', async () => {
    await showConfirm('Delete?', 'This cannot be undone')
    const msg = (confirm as ReturnType<typeof vi.fn>).mock.calls[0][0] as string
    expect(msg).toContain('Delete?')
    expect(msg).toContain('This cannot be undone')
  })
})
