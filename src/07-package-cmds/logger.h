#pragma once

#include "event.h"
#include "listener.h"

class Logger : public EventListener<PackEvent> {
 public:
  void onEvent(const PackEvent&) override;
};