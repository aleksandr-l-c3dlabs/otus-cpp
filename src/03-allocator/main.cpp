#include "allocator.h"
#include "container.h"
#include "lib.h"

#include <cassert>
#include <iostream>
#include <map>
#include <string_view>

int factorial(int n) {
  return n == 0 ? 1 : n * factorial(n - 1);
}

template <class T, class U, class C, class Alloc>
void fill_map(std::map<T, U, C, Alloc>& m) {
  for (int i = 0; i < 10; ++i) {
    m[i] = factorial(i);
  }
}

template <class T, class U, class C, class Alloc>
void output_map(std::string_view header, const std::map<T, U, C, Alloc>& m) {
  std::cout << header << ":" << std::endl;
  for (const auto& v : m) {
    std::cout << "\t" << v.first << " " << v.second << std::endl;
  }
}

template <typename Alloc>
void fill_container(stl_container<int, Alloc>& c) {
  for (int i = 0; i < 10; ++i) {
    c.push_back(i);
  }
}

template <class T, class Alloc>
void output_container(std::string_view header,
                      const stl_container<T, Alloc>& c) {
  std::cout << header << ": ";
  for (const auto& v : c) {
    std::cout << v << " ";
  }
  std::cout << std::endl;
}

int main() {
  try {
    // 1. create std::map<int, int>
    std::map<int, int> std_map;

    // 2. fill 10 elements by factorial values
    fill_map(std_map);

    // 3. create std::map<int, int> with custom allocator (limited by 10 elements)
    std::map<int, int, std::less<int>,
             stl_allocator<std::pair<const int, int>, 10>>
        stl_map;

    // 4. fill 10 elements by factorial values
    fill_map(stl_map);

    // 5. output map pair values
    output_map("map with a standard allocator", std_map);
    output_map("map with a custom allocator", stl_map);

    // 6. create a custom container
    stl_container<int> int_container;

    // 7. fill container
    fill_container(int_container);
    output_container("container values", int_container);

    // 8. create a custom container with a custom allocator
    stl_container<int, stl_allocator<int, 10>> int_alloc_container;

    // 9. file container
    fill_container(int_alloc_container);

    // 10. output container values
    output_container("container values", int_container);
    output_container("container with custom allocator values",
                     int_alloc_container);

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}