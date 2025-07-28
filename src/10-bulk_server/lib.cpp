#include "lib.h"

#include <async.h>

Application::Application(std::size_t bulk_size) {
  default_context = connect(bulk_size);
}

Application::~Application() {
  disconnect(default_context);
}

void* Application::handle_command(const std::string& cmd, void* context) {
  void* result = nullptr;

  if (cmd == "{" && !context) {
    result = connect(0);
  } else {
    result = context ? context : default_context;
  }

  receive(cmd.data(), cmd.size(), result);

  if (cmd == "}" && context) {
    // TODO: may be problem with multiple '}'
    disconnect(context);
    result = nullptr;
  } else if (result == default_context) {
    result = nullptr;
  }

  return result;
}