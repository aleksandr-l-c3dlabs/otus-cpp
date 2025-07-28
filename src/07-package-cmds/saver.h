#pragma once

#include "events.h"
#include "listener.h"

class Saver : public EventListener<PackEvent> {
 public:
  Saver(bool randomPostfix = false);

  void onEvent(const PackEvent&) override;

 private:
  bool randomPostfix;
};