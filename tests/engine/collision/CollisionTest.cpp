#include <cmath>
#include <eng/collision/Collision.hpp>
#include <gtest/gtest.h>

namespace
{

constexpr eng::Aabb wideBrick{.pos = {200.f, 80.f}, .size = {110.f, 28.f}};

} // namespace

TEST(IntersectShould, BottomHitYieldsDownwardNormal)
{
    // Brick bottom at y=108; center just below overlapping from below (+y down).
    const eng::Circle circle{.center = {247.f, 110.f}, .radius = 8.f};

    const auto contact = eng::intersect(circle, wideBrick);
    ASSERT_TRUE(contact.has_value());
    EXPECT_GT(contact->normal.y, 0.f);
    EXPECT_LT(std::fabs(contact->normal.x), std::fabs(contact->normal.y));
}

TEST(IntersectShould, SideHitYieldsHorizontalNormal)
{
    // Brick right edge at x=310; center just to the right, overlapping.
    const eng::Circle circle{.center = {314.f, 94.f}, .radius = 8.f};

    const auto contact = eng::intersect(circle, wideBrick);
    ASSERT_TRUE(contact.has_value());
    EXPECT_GT(contact->normal.x, 0.f);
    EXPECT_LT(std::fabs(contact->normal.y), std::fabs(contact->normal.x));
}

TEST(IntersectShould, CornerOverlapGivesUnitDiagonal)
{
    // Bottom-right corner at (310, 108).
    const eng::Circle circle{.center = {314.f, 112.f}, .radius = 8.f};

    const auto contact = eng::intersect(circle, wideBrick);
    ASSERT_TRUE(contact.has_value());
    EXPECT_NEAR(eng::lengthSq(contact->normal), 1.f, 1e-4f);
    EXPECT_GT(contact->normal.x, 0.f);
    EXPECT_GT(contact->normal.y, 0.f);
}

TEST(IntersectShould, InsideUsesMinimumPenetrationAxis)
{
    // Closer to the bottom edge (bottom at 108) than to other edges.
    const eng::Circle circle{.center = {255.f, 100.f}, .radius = 8.f};

    const auto contact = eng::intersect(circle, wideBrick);
    ASSERT_TRUE(contact.has_value());
    EXPECT_NEAR(contact->normal.x, 0.f, 1e-5f);
    EXPECT_NEAR(contact->normal.y, 1.f, 1e-5f);
    EXPECT_NEAR(contact->depth, (108.f - 100.f) + 8.f, 1e-4f);
}

TEST(IntersectShould, MissReturnsNullopt)
{
    const eng::Circle circle{.center = {247.f, 200.f}, .radius = 8.f};

    EXPECT_FALSE(eng::intersect(circle, wideBrick).has_value());
}

TEST(SweepShould, HitsBrickWhileMovingUp)
{
    const eng::Aabb brick{.pos = {200.f, 80.f}, .size = {110.f, 28.f}};
    const eng::Circle circle{.center = {247.f, 200.f}, .radius = 8.f};

    const auto hit = eng::sweep(circle, {0.f, -400.f}, brick);
    ASSERT_TRUE(hit.has_value());
    EXPECT_GT(hit->time, 0.f);
    EXPECT_LE(hit->time, 1.f);
    EXPECT_NEAR(hit->normal.x, 0.f, 1e-5f);
    EXPECT_NEAR(hit->normal.y, 1.f, 1e-5f);
}

TEST(ReflectShould, KeepsVelocityWhenMovingAway)
{
    const eng::Vec2f velocity{0.f, 100.f};
    const eng::Vec2f normal{0.f, 1.f};

    const eng::Vec2f reflected = eng::reflect(velocity, normal);
    EXPECT_FLOAT_EQ(reflected.x, velocity.x);
    EXPECT_FLOAT_EQ(reflected.y, velocity.y);
}

TEST(ReflectShould, BouncesWhenMovingIntoSurface)
{
    const eng::Vec2f velocity{0.f, -100.f};
    const eng::Vec2f normal{0.f, 1.f};

    const eng::Vec2f reflected = eng::reflect(velocity, normal);
    EXPECT_FLOAT_EQ(reflected.x, 0.f);
    EXPECT_FLOAT_EQ(reflected.y, 100.f);
}
