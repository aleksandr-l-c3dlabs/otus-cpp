#pragma once

#include <atomic>
#include <sstream>
#include "events.h"
#include "provider.h"

class Reader : public EventProvider<InputCommandEvent> {
 public:
  Reader();
  ~Reader();

  void run(std::istream& input);

 private:
  static std::atomic<bool> interrupted;
  static std::istream* in;

  static void signalHandler(int code);
};
