#pragma once

template <typename Event>
class EventListener {
 public:
  virtual ~EventListener() = default;

  virtual void onEvent(const Event&) = 0;
};
