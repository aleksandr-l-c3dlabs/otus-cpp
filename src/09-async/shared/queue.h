#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

template <typename T>
struct ThreadSafeQueue {
 public:
  void push(T value) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(std::move(value));
    cond.notify_one();
  }

  std::optional<T> pop() {
    std::unique_lock<std::mutex> lock(mutex);
    cond.wait(lock, [this] { return !queue.empty() || stopped; });

    if (stopped && queue.empty()) {
      return std::nullopt;
    }

    T value = std::move(queue.front());
    queue.pop();
    return value;
  }

  void stop() {
    {
      std::lock_guard<std::mutex> lock(mutex);
      stopped = true;
    }
    cond.notify_all();
  }

  bool empty() const {
    std::lock_guard<std::mutex> lock(mutex);
    return queue.empty();
  }

 private:
  mutable std::mutex mutex;
  std::condition_variable cond;
  std::queue<T> queue;
  bool stopped = false;
};
