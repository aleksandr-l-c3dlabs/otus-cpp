#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include "listener.h"

template <typename Event>
class EventProvider {
 public:
  void addListener(std::shared_ptr<EventListener<Event>> listener) {
    listeners.push_back(std::weak_ptr<EventListener<Event>>(listener));
  }

  void notifyListeners(const Event& event) {
    listeners.erase(
        std::remove_if(listeners.begin(), listeners.end(),
                       [](const auto& weak) { return weak.expired(); }),
        listeners.end());

    for (auto& weak : listeners) {
      if (auto listener = weak.lock()) {
        listener->onEvent(event);
      }
    }
  }

 private:
  std::vector<std::weak_ptr<EventListener<Event>>> listeners;
};