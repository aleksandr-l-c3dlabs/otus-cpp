#include <gtest/gtest.h>

#include <lib.h>
#include <list>
#include <string>
#include <tuple>
#include <vector>

template <typename T>
std::string print_ip_output(const T& value) {
  testing::internal::CaptureStdout();
  print_ip(value);
  return testing::internal::GetCapturedStdout();
}

TEST(lib, int8) {
  EXPECT_EQ(print_ip_output(int8_t{-1}), "255\n");
}

TEST(lib, int16) {
  EXPECT_EQ(print_ip_output(int16_t{0}), "0.0\n");
}

TEST(lib, int32) {
  EXPECT_EQ(print_ip_output(int32_t{2130706433}), "127.0.0.1\n");
}

TEST(lib, int64) {
  EXPECT_EQ(print_ip_output(int64_t{8875824491850138409}),
            "123.45.67.89.101.112.131.41\n");
}

TEST(lib, string) {
  EXPECT_EQ(print_ip_output(std::string{"Hello, World!"}), "Hello, World!\n");
}

TEST(lib, vector_list) {
  EXPECT_EQ(print_ip_output(std::vector<int>{100, 200, 300, 400}),
            "100.200.300.400\n");
  EXPECT_EQ(print_ip_output(std::list<short>{400, 300, 200, 100}),
            "400.300.200.100\n");
}

TEST(lib, tuple) {
  EXPECT_EQ(print_ip_output(std::make_tuple(123, 456, 789, 0)),
            "123.456.789.0\n");
}