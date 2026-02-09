#pragma once

#include <nlohmann/json.hpp>
#include <functional>
#include <future>
#include <mutex>
#include <queue>

using json = nlohmann::json;
using WorkItem = std::packaged_task<json()>;

class MainThreadDispatch {
public:
  // Called from HTTP thread — blocks until main thread executes the work
  template <typename F> static auto Run(F &&fn) -> decltype(fn()) {
    using ReturnType = decltype(fn());

    auto task = std::packaged_task<ReturnType()>(std::forward<F>(fn));
    auto future = task.get_future();

    {
      std::lock_guard<std::mutex> lock(queueMutex_);
      workQueue_.push(std::move(task));
    }

    // Block until main thread picks it up
    return future.get();
  }

  // Called from AITimerSuite callback (main thread, every ~16ms)
  static void ProcessQueue();

private:
  static std::mutex queueMutex_;
  static std::queue<std::packaged_task<json()>> workQueue_;
};

// Convenience macro for generated code
#define RunOnMainThread(fn) MainThreadDispatch::Run(fn)
