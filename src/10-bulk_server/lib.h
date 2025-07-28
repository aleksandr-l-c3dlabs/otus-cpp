#pragma once

#include <stddef.h>
#include <string>

class Application {
 public:
  Application(std::size_t bulk_size);
  ~Application();

  void* handle_command(const std::string& cmd, void* context = nullptr);

 private:
  void* default_context;
};