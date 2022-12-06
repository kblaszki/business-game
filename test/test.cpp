//
// Created by Dev on 07.06.2022.
//
#include "../src/LibraryCode.h"

#include <gtest/gtest.h>

TEST(SimpleTest, testSomething) {
  int result = sum(2, 4);
  ASSERT_EQ(6, result);
}