#include "lib.h"

#include <cassert>
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv) {
  if (argc < 2)
    return 1;
  try {
    Application app;
    app.run(atoi(argv[1]));
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}