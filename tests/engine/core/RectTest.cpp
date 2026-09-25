#include <gtest/gtest.h>
#include <sgl/core/Rect.hpp>

TEST(RectTest, ContainsIsHalfOpen)
{
    const sgl::Rect<float> rect{.pos = {0.f, 0.f}, .size = {10.f, 10.f}};

    EXPECT_TRUE(rect.contains({0.f, 0.f}));
    EXPECT_TRUE(rect.contains({5.f, 5.f}));
    EXPECT_TRUE(rect.contains({9.f, 9.f}));
    EXPECT_FALSE(rect.contains({10.f, 5.f}));
    EXPECT_FALSE(rect.contains({5.f, 10.f}));
    EXPECT_FALSE(rect.contains({10.f, 10.f}));
}

TEST(RectTest, EdgeTouchingDoesNotIntersect)
{
    const sgl::Rect<float> a{.pos = {0.f, 0.f}, .size = {10.f, 10.f}};
    const sgl::Rect<float> b{.pos = {10.f, 0.f}, .size = {10.f, 10.f}};
    const sgl::Rect<float> c{.pos = {0.f, 10.f}, .size = {10.f, 10.f}};

    EXPECT_FALSE(a.intersects(b));
    EXPECT_FALSE(b.intersects(a));
    EXPECT_FALSE(a.intersects(c));
    EXPECT_FALSE(c.intersects(a));
}

TEST(RectTest, OverlappingRectsIntersect)
{
    const sgl::Rect<float> a{.pos = {0.f, 0.f}, .size = {10.f, 10.f}};
    const sgl::Rect<float> b{.pos = {5.f, 5.f}, .size = {10.f, 10.f}};

    EXPECT_TRUE(a.intersects(b));
    EXPECT_TRUE(b.intersects(a));
}
