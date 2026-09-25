#include <gtest/gtest.h>
#include <sgl/core/Vec2.hpp>
#include <sgl/core/Vec3.hpp>

static_assert(sgl::Vec2f{1, 2} + sgl::Vec2f{3, 4} == sgl::Vec2f{4, 6});

TEST(VecTest, Dot)
{
    EXPECT_FLOAT_EQ(sgl::dot(sgl::Vec2f{3, 4}, sgl::Vec2f{2, 1}), 10.f);
    EXPECT_FLOAT_EQ(sgl::dot(sgl::Vec3f{1, 2, 3}, sgl::Vec3f{4, 5, 6}), 32.f);
}

TEST(VecTest, LengthSq)
{
    EXPECT_FLOAT_EQ(sgl::lengthSq(sgl::Vec2f{3, 4}), 25.f);
    EXPECT_FLOAT_EQ(sgl::lengthSq(sgl::Vec3f{1, 2, 2}), 9.f);
}

TEST(VecTest, NormalizedZeroIsNullopt)
{
    EXPECT_FALSE(sgl::normalized(sgl::Vec2f{}).has_value());
    EXPECT_FALSE(sgl::normalized(sgl::Vec3f{}).has_value());
}

TEST(VecTest, NormalizedUnit)
{
    const auto n2 = sgl::normalized(sgl::Vec2f{3, 4});
    ASSERT_TRUE(n2.has_value());
    EXPECT_NEAR(n2->x, 0.6f, 1e-6f);
    EXPECT_NEAR(n2->y, 0.8f, 1e-6f);
    EXPECT_NEAR(sgl::lengthSq(*n2), 1.f, 1e-6f);

    const auto n3 = sgl::normalized(sgl::Vec3f{0, 3, 4});
    ASSERT_TRUE(n3.has_value());
    EXPECT_NEAR(n3->x, 0.f, 1e-6f);
    EXPECT_NEAR(n3->y, 0.6f, 1e-6f);
    EXPECT_NEAR(n3->z, 0.8f, 1e-6f);
}
