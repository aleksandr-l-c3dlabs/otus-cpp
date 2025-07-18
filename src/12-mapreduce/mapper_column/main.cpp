#include <iostream>
#include "lib.h"

int main() {
  if (!get_column(std::cin, std::cout, 9, ','))
    return 1;

  return 0;
}