#include "logger.h"

#include <iostream>

void Logger::onEvent(const PackEvent& event) {
  bool first = true;
  for (const auto& cmd : event.commands) {
    std::cout << (first ? "bulk:" : ",");
    std::cout << " " << cmd;
    if (first)
      first = false;
  }
  std::cout << std::endl;
}
