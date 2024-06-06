/* Created by kblaszki */

#include <sum-library/LibraryCode.hpp>

#include <gtest/gtest.h>

TEST(SimpleTest, testSomething)
{
    int result = sum(2, 4);
    ASSERT_EQ(6, result);
}