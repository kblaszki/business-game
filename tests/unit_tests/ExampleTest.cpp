#include <Example.hpp>
#include <gtest/gtest.h>

TEST(ExampleShould, addTwoNumbers)
{
    EXPECT_EQ(Example{}.add(2, 3), 5);
}
