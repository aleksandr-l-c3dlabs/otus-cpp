#pragma once

#include <ctime>
#include <string>
#include <vector>

struct InputCommandEvent {
  std::string command;
  bool interrupted;
};

struct PackEvent {
  std::time_t created;
  std::vector<std::string> commands;
};
