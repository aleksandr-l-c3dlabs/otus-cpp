#pragma once

#include <events.h>
#include <listener.h>
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include "queue.h"

using Queue = ThreadSafeQueue<PackEvent>;

class ThreadListener : public EventListener<PackEvent> {
 public:
  ThreadListener();
  ~ThreadListener();

  void onEvent(const PackEvent& pack) override {
    log_queue->push(pack);
    save_queue->push(pack);
  }

 private:
  std::atomic_bool running{true};
  std::thread log_thread, save1_thread, save2_thread;
  std::shared_ptr<Queue> log_queue;
  std::shared_ptr<Queue> save_queue;
};