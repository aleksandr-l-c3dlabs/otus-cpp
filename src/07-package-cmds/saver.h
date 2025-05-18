#pragma once

#include "event.h"
#include "listener.h"

class Saver : public EventListener<PackEvent> {
 public:
  Saver();

  void onEvent(const PackEvent&) override;

 private:
};