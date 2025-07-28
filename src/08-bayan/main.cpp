#include <exception>
#include <iostream>

#include "app.h"

int main(const int argc, const char* argv[]) {
  try {
    Application app(argc, argv);
    if (app.is_invalid()) {
      app.print_invalid_arg(std::cout);
      app.print_help(std::cout);
    } else if (app.is_help()) {
      app.print_help(std::cout);
    } else {
      for (const auto& group : app.diff_groups()) {
        for (const auto& path : group)
          std::cout << path << std::endl;
        std::cout << std::endl;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}