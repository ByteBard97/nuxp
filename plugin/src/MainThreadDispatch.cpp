#include "MainThreadDispatch.hpp"

std::mutex MainThreadDispatch::queueMutex_;
std::queue<std::packaged_task<json()>> MainThreadDispatch::workQueue_;

void MainThreadDispatch::ProcessQueue() {
  std::lock_guard<std::mutex> lock(queueMutex_);
  while (!workQueue_.empty()) {
    auto task = std::move(workQueue_.front());
    workQueue_.pop();
    task(); // executes on main thread
  }
}
