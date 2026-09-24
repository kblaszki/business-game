#include <eng/core/Vec2.hpp>
#include <eng/core/Vec3.hpp>

#include <gtest/gtest.h>

static_assert(eng::Vec2f{1, 2} + eng::Vec2f{3, 4} == eng::Vec2f{4, 6});

TEST(VecTest, Dot)
{
    EXPECT_FLOAT_EQ(eng::dot(eng::Vec2f{3, 4}, eng::Vec2f{2, 1}), 10.f);
    EXPECT_FLOAT_EQ(eng::dot(eng::Vec3f{1, 2, 3}, eng::Vec3f{4, 5, 6}), 32.f);
}

TEST(VecTest, LengthSq)
{
    EXPECT_FLOAT_EQ(eng::lengthSq(eng::Vec2f{3, 4}), 25.f);
    EXPECT_FLOAT_EQ(eng::lengthSq(eng::Vec3f{1, 2, 2}), 9.f);
}

TEST(VecTest, NormalizedZeroIsNullopt)
{
    EXPECT_FALSE(eng::normalized(eng::Vec2f{}).has_value());
    EXPECT_FALSE(eng::normalized(eng::Vec3f{}).has_value());
}

TEST(VecTest, NormalizedUnit)
{
    const auto n2 = eng::normalized(eng::Vec2f{3, 4});
    ASSERT_TRUE(n2.has_value());
    EXPECT_NEAR(n2->x, 0.6f, 1e-6f);
    EXPECT_NEAR(n2->y, 0.8f, 1e-6f);
    EXPECT_NEAR(eng::lengthSq(*n2), 1.f, 1e-6f);

    const auto n3 = eng::normalized(eng::Vec3f{0, 3, 4});
    ASSERT_TRUE(n3.has_value());
    EXPECT_NEAR(n3->x, 0.f, 1e-6f);
    EXPECT_NEAR(n3->y, 0.6f, 1e-6f);
    EXPECT_NEAR(n3->z, 0.8f, 1e-6f);
}
