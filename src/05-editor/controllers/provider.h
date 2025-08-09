#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include "listener.h"

/// @brief Base class for event providing
/// @tparam Event Event struct
template <typename Event>
class EventProvider {
 public:
  /// @brief Registry a new event listener
  /// @param[in] listener Event listener
  void addListener(std::shared_ptr<EventListener<Event>> listener) {
    listeners.push_back(std::weak_ptr<EventListener<Event>>(listener));
  }

  /// @brief Run all event handler
  /// @param[in] event Event struct
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
  /// @brief A list of all registered event listeners
  std::vector<std::weak_ptr<EventListener<Event>>> listeners;
};