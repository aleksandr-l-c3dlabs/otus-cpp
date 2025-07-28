#include "thread-listener.h"

#include <logger.h>
#include <saver.h>

void worker(std::atomic_bool& running, std::shared_ptr<Queue> queue,
            std::shared_ptr<EventListener<PackEvent>> listener) {
  while (running) {
    auto pack = queue->pop();
    if (pack.has_value()) {
      listener->onEvent(pack.value());
    }
  }
}

ThreadListener::ThreadListener()
    : log_queue(std::make_shared<Queue>()),
      save_queue(std::make_shared<Queue>()) {
  auto logger = std::make_shared<Logger>();
  auto saver1 = std::make_shared<Saver>(true);
  auto saver2 = std::make_shared<Saver>(true);

  log_thread =
      std::thread(worker, std::ref(running), log_queue, std::move(logger));
  save1_thread =
      std::thread(worker, std::ref(running), save_queue, std::move(saver1));
  save2_thread =
      std::thread(worker, std::ref(running), save_queue, std::move(saver2));
}

ThreadListener::~ThreadListener() {
  running = false;
  log_queue->stop();
  save_queue->stop();

  if (log_thread.joinable())
    log_thread.join();
  if (save1_thread.joinable())
    save1_thread.join();
  if (save2_thread.joinable())
    save2_thread.join();
}