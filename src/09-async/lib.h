#pragma once

#include <cstddef>

class AsyncDemo {
 public:
  AsyncDemo();
  ~AsyncDemo();

  void run(std::size_t pack_size);
};