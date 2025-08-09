#pragma once

/// @brief Base event listener class
/// @tparam Event Event struct
template <typename Event>
class EventListener {
 public:
  virtual ~EventListener() = default;

  /// @brief Event handler
  /// @param[in] event Event struct
  virtual void onEvent(const Event&) = 0;
};
