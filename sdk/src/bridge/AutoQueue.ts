/**
 * AutoQueue - A self-processing asynchronous task queue
 *
 * Ensures that operations sent to the plugin host are executed sequentially,
 * preventing race conditions caused by rapid async calls overwhelming
 * the single-threaded host environment.
 */

interface QueueItem<T> {
  action: () => Promise<T>
  resolve: (value: T | PromiseLike<T>) => void
  reject: (reason?: unknown) => void
}

export class AutoQueue {
  private _items: QueueItem<unknown>[] = []
  private _pendingPromise: boolean = false

  /**
   * Enqueues an action and returns a Promise that resolves with the action's result.
   * @param action A function that returns a Promise
   * @returns A Promise that resolves when the action completes
   */
  public enqueue<T>(action: () => Promise<T>): Promise<T> {
    return new Promise<T>((resolve, reject) => {
      this._items.push({ action, resolve, reject } as QueueItem<unknown>)
      this.dequeue()
    })
  }

  /**
   * Processes the next item in the queue if not already busy.
   */
  private async dequeue(): Promise<void> {
    if (this._pendingPromise) {
      return
    }

    const item = this._items.shift()
    if (!item) {
      return
    }

    try {
      this._pendingPromise = true
      const payload = await item.action()
      item.resolve(payload)
    } catch (e) {
      item.reject(e)
    } finally {
      this._pendingPromise = false
      this.dequeue()
    }
  }

  /** Returns the current queue length */
  public get length(): number {
    return this._items.length
  }

  /** Returns whether a task is currently being processed */
  public get isBusy(): boolean {
    return this._pendingPromise
  }
}
