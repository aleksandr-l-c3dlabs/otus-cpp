#include <gtest/gtest.h>

#include <lib.h>

TEST(lib, version_positive) {
  EXPECT_TRUE(version() > 0);
}
