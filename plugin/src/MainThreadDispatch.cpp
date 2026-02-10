/**
 * Main Thread Dispatch Implementation
 *
 * Provides thread-safe work queue for dispatching SDK calls from
 * HTTP server threads to Illustrator's main thread.
 */

#include "MainThreadDispatch.hpp"

/*******************************************************************************
 * Static Member Definitions
 ******************************************************************************/

std::mutex MainThreadDispatch::queueMutex_;
std::queue<std::shared_ptr<WorkItem>> MainThreadDispatch::workQueue_;

/*******************************************************************************
 * MainThreadDispatch::Push
 ******************************************************************************/

void MainThreadDispatch::Push(std::shared_ptr<WorkItem> item) {
  std::lock_guard<std::mutex> lock(queueMutex_);
  workQueue_.push(std::move(item));
}

/*******************************************************************************
 * MainThreadDispatch::HasWork
 ******************************************************************************/

bool MainThreadDispatch::HasWork() {
  std::lock_guard<std::mutex> lock(queueMutex_);
  return !workQueue_.empty();
}

/*******************************************************************************
 * MainThreadDispatch::QueueSize
 ******************************************************************************/

size_t MainThreadDispatch::QueueSize() {
  std::lock_guard<std::mutex> lock(queueMutex_);
  return workQueue_.size();
}

/*******************************************************************************
 * MainThreadDispatch::ProcessQueue
 ******************************************************************************/

void MainThreadDispatch::ProcessQueue() {
  // Take all items from the queue under lock
  std::queue<std::shared_ptr<WorkItem>> items;
  {
    std::lock_guard<std::mutex> lock(queueMutex_);
    std::swap(items, workQueue_);
  }

  // Process items outside the lock
  while (!items.empty()) {
    auto item = std::move(items.front());
    items.pop();

    // Execute the work
    json result;
    try {
      result = item->work();
    } catch (const std::exception &e) {
      result = json{{"success", false}, {"error", e.what()}};
    } catch (...) {
      result = json{{"success", false}, {"error", "Unknown exception"}};
    }

    // Signal completion
    // The shared_ptr ensures this is safe even if the caller timed out
    {
      std::lock_guard<std::mutex> lock(*item->mutex);
      *item->result = std::move(result);
      *item->completed = true;
    }
    item->cv->notify_one();
  }
}

/*******************************************************************************
 * MainThreadDispatch::Clear
 ******************************************************************************/

void MainThreadDispatch::Clear() {
  std::lock_guard<std::mutex> lock(queueMutex_);
  // Swap with empty queue (more efficient than popping each element)
  std::queue<std::shared_ptr<WorkItem>> empty;
  std::swap(workQueue_, empty);
}

