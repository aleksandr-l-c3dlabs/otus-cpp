#pragma once

#include <iostream>
#include <string>
#include <tuple>

void example_usage();

template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
void print_ip(const T& value) {
  const auto* bytes = reinterpret_cast<const unsigned char*>(&value);
  for (size_t i = sizeof(T); i > 0; --i) {
    if (i != sizeof(T)) {
      std::cout << ".";
    }
    std::cout << static_cast<int>(bytes[i - 1]);
  }
  std::cout << std::endl;
}

void print_ip(const std::string& value) {
  std::cout << value << std::endl;
}

template <typename T>
auto print_ip(const T& value) -> decltype(value.begin(), value.end(), void()) {
  bool is_first = true;
  for (const auto& part : value) {
    if (!is_first)
      std::cout << '.';
    else
      is_first = false;
    std::cout << part;
  }
  std::cout << std::endl;
}

template <typename T>
struct all_same_type;

template <typename T, typename... Ts>
struct all_same_type<std::tuple<T, Ts...>> {
  static constexpr bool value = std::conjunction_v<std::is_same<T, Ts>...>;
};

template <typename T>
constexpr bool all_same_type_v = all_same_type<T>::value;

template <typename... Ts>
auto print_ip(const std::tuple<Ts...>& value)
    -> std::enable_if_t<all_same_type_v<std::tuple<Ts...>>, void> {
  std::apply(
      [](const Ts&... args) {
        std::size_t n{0};
        ((std::cout << args << (++n != sizeof...(args) ? "." : "")), ...);
      },
      value);
  std::cout << std::endl;
}
