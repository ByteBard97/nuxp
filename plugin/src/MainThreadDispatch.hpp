#pragma once

/**
 * Main Thread Dispatch
 *
 * Provides safe execution of SDK calls from the HTTP server's background
 * thread onto Illustrator's main thread. All Adobe SDK calls must be made
 * from the main thread.
 *
 * Thread Safety:
 *   This implementation uses shared_ptr for synchronization primitives to
 *   prevent use-after-free when timeouts occur. If the HTTP request times
 *   out before the main thread processes the work, the shared_ptr prevents
 *   the main thread from accessing freed memory.
 *
 * Usage:
 *   // From HTTP thread:
 *   auto result = MainThreadDispatch::Run([&]() -> json {
 *       // This runs on main thread
 *       return callSdkFunction();
 *   });
 *
 *   // With timeout:
 *   auto result = MainThreadDispatch::RunWithTimeout([&]() -> json {
 *       return callSdkFunction();
 *   }, std::chrono::seconds(5));
 */

#include <nlohmann/json.hpp>

#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>

using json = nlohmann::json;

/**
 * Work Item
 *
 * Encapsulates a unit of work to be executed on the main thread.
 * Uses shared_ptr for synchronization primitives to handle timeout safely.
 */
struct WorkItem {
  std::function<json()> work;
  std::shared_ptr<json> result;
  std::shared_ptr<std::mutex> mutex;
  std::shared_ptr<std::condition_variable> cv;
  std::shared_ptr<bool> completed;

  WorkItem()
      : result(std::make_shared<json>()),
        mutex(std::make_shared<std::mutex>()),
        cv(std::make_shared<std::condition_variable>()),
        completed(std::make_shared<bool>(false)) {}
};

class MainThreadDispatch {
public:
  /**
   * Run work on the main thread and block until complete.
   * This is a convenience wrapper that calls RunWithTimeout with infinite
   * timeout.
   *
   * @param fn The function to execute on the main thread
   * @return The result of the function
   */
  template <typename F> static auto Run(F &&fn) -> decltype(fn()) {
    using ReturnType = decltype(fn());
    static_assert(std::is_same_v<ReturnType, json>,
                  "Work function must return json");

    auto item = std::make_shared<WorkItem>();
    item->work = std::forward<F>(fn);

    Push(item);

    // Wait indefinitely
    std::unique_lock<std::mutex> lock(*item->mutex);
    item->cv->wait(lock, [&item] { return *item->completed; });

    return *item->result;
  }

  /**
   * Run work on the main thread with a timeout.
   *
   * @param fn The function to execute on the main thread
   * @param timeout Maximum time to wait for completion
   * @return Optional result - empty if timeout occurred
   */
  template <typename F, typename Rep, typename Period>
  static std::optional<json>
  RunWithTimeout(F &&fn, std::chrono::duration<Rep, Period> timeout) {
    auto item = std::make_shared<WorkItem>();
    item->work = std::forward<F>(fn);

    Push(item);

    // Wait with timeout
    std::unique_lock<std::mutex> lock(*item->mutex);
    bool completed =
        item->cv->wait_for(lock, timeout, [&item] { return *item->completed; });

    if (completed) {
      return *item->result;
    }

    // Timeout occurred - the work may still be executed later,
    // but the shared_ptr ensures safe memory access
    return std::nullopt;
  }

  /**
   * Check if there is pending work in the queue.
   */
  static bool HasWork();

  /**
   * Get the number of pending work items.
   */
  static size_t QueueSize();

  /**
   * Process all pending work items.
   * Called from AITimerSuite callback (main thread, every ~16ms).
   */
  static void ProcessQueue();

  /**
   * Clear all pending work items.
   * Use with caution - pending callers will wait indefinitely.
   */
  static void Clear();

private:
  static void Push(std::shared_ptr<WorkItem> item);

  static std::mutex queueMutex_;
  static std::queue<std::shared_ptr<WorkItem>> workQueue_;
};

// Convenience macro for generated code
#define RunOnMainThread(fn) MainThreadDispatch::Run(fn)

