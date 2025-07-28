#pragma once

#include <ctime>
#include <string>
#include <vector>
#include "events.h"
#include "listener.h"
#include "provider.h"

class Packer : public EventProvider<PackEvent>,
               public EventListener<InputCommandEvent> {
 public:
  Packer(size_t size) : pack_size(size), level(0) { collection.reserve(size); }

  void onEvent(const InputCommandEvent&) override;

 private:
  void packCollection();

  size_t pack_size;
  size_t level;
  std::time_t created;
  std::vector<std::string> collection;
};
