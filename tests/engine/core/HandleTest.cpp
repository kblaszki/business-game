#include <eng/core/EntityId.hpp>
#include <eng/core/Handle.hpp>
#include <gtest/gtest.h>
#include <unordered_set>

TEST(HandleTest, OrderingAndEquality)
{
    using H = eng::Handle<struct TestTag>;

    const H a{.id = 1};
    const H b{.id = 2};
    const H a2{.id = 1};

    EXPECT_EQ(a, a2);
    EXPECT_NE(a, b);
    EXPECT_LT(a, b);
    EXPECT_GT(b, a);
}

TEST(HandleTest, UnorderedSetInsertFind)
{
    using H = eng::Handle<struct TestTag>;

    std::unordered_set<H> set;
    const H key{.id = 42};

    EXPECT_TRUE(set.insert(key).second);
    EXPECT_NE(set.find(key), set.end());
    EXPECT_EQ(set.find(H{.id = 7}), set.end());
}

TEST(HandleTest, EntityIdEquality)
{
    const eng::EntityId a{.index = 1, .generation = 2};
    const eng::EntityId b{.index = 1, .generation = 2};
    const eng::EntityId c{.index = 1, .generation = 3};

    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}
