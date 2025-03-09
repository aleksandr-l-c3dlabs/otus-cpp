#include "lib.h"

#include <cassert>
#include <iostream>

int main() {
  try {
    ip_filter(std::cin, std::cout);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}