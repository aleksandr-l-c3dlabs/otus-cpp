#pragma once

#include <events.h>
#include <provider.h>
#include <string>

class Receiver : public EventProvider<InputCommandEvent> {
 public:
  Receiver() {}
  ~Receiver() { notifyListeners(InputCommandEvent{"", true}); }

  void receive(const std::string& cmd) {
    notifyListeners(InputCommandEvent{cmd, false});
  }
};
