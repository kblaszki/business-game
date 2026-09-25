#include <cmath>
#include <gtest/gtest.h>
#include <sgl/collision/Collision.hpp>

namespace
{

constexpr sgl::Aabb wideBrick{.pos = {200.f, 80.f}, .size = {110.f, 28.f}};

} // namespace

TEST(IntersectShould, BottomHitYieldsDownwardNormal)
{
    // Brick bottom at y=108; center just below overlapping from below (+y down).
    const sgl::Circle circle{.center = {247.f, 110.f}, .radius = 8.f};

    const auto contact = sgl::intersect(circle, wideBrick);
    ASSERT_TRUE(contact.has_value());
    EXPECT_GT(contact->normal.y, 0.f);
    EXPECT_LT(std::fabs(contact->normal.x), std::fabs(contact->normal.y));
}

TEST(IntersectShould, SideHitYieldsHorizontalNormal)
{
    // Brick right edge at x=310; center just to the right, overlapping.
    const sgl::Circle circle{.center = {314.f, 94.f}, .radius = 8.f};

    const auto contact = sgl::intersect(circle, wideBrick);
    ASSERT_TRUE(contact.has_value());
    EXPECT_GT(contact->normal.x, 0.f);
    EXPECT_LT(std::fabs(contact->normal.y), std::fabs(contact->normal.x));
}

TEST(IntersectShould, CornerOverlapGivesUnitDiagonal)
{
    // Bottom-right corner at (310, 108).
    const sgl::Circle circle{.center = {314.f, 112.f}, .radius = 8.f};

    const auto contact = sgl::intersect(circle, wideBrick);
    ASSERT_TRUE(contact.has_value());
    EXPECT_NEAR(sgl::lengthSq(contact->normal), 1.f, 1e-4f);
    EXPECT_GT(contact->normal.x, 0.f);
    EXPECT_GT(contact->normal.y, 0.f);
}

TEST(IntersectShould, InsideUsesMinimumPenetrationAxis)
{
    // Closer to the bottom edge (bottom at 108) than to other edges.
    const sgl::Circle circle{.center = {255.f, 100.f}, .radius = 8.f};

    const auto contact = sgl::intersect(circle, wideBrick);
    ASSERT_TRUE(contact.has_value());
    EXPECT_NEAR(contact->normal.x, 0.f, 1e-5f);
    EXPECT_NEAR(contact->normal.y, 1.f, 1e-5f);
    EXPECT_NEAR(contact->depth, (108.f - 100.f) + 8.f, 1e-4f);
}

TEST(IntersectShould, MissReturnsNullopt)
{
    const sgl::Circle circle{.center = {247.f, 200.f}, .radius = 8.f};

    EXPECT_FALSE(sgl::intersect(circle, wideBrick).has_value());
}

TEST(SweepShould, HitsBrickWhileMovingUp)
{
    const sgl::Aabb brick{.pos = {200.f, 80.f}, .size = {110.f, 28.f}};
    const sgl::Circle circle{.center = {247.f, 200.f}, .radius = 8.f};

    const auto hit = sgl::sweep(circle, {0.f, -400.f}, brick);
    ASSERT_TRUE(hit.has_value());
    EXPECT_GT(hit->time, 0.f);
    EXPECT_LE(hit->time, 1.f);
    EXPECT_NEAR(hit->normal.x, 0.f, 1e-5f);
    EXPECT_NEAR(hit->normal.y, 1.f, 1e-5f);
}

TEST(SweepShould, missCornerOutsideRoundedRegion)
{
    // Clips the expanded-AABB corner square past the quarter-circle at (310, 108).
    // Old point-vs-expanded-box sweep false-hits here with an axis normal.
    const sgl::Circle circle{.center = {326.f, 104.f}, .radius = 8.f};
    const auto hit = sgl::sweep(circle, {-24.f, 24.f}, wideBrick);
    EXPECT_FALSE(hit.has_value());
}

TEST(SweepShould, hitCornerWithDiagonalNormal)
{
    // Approaches the bottom-right corner through the rounded region.
    const sgl::Circle circle{.center = {330.f, 128.f}, .radius = 8.f};
    const auto hit = sgl::sweep(circle, {-40.f, -40.f}, wideBrick);
    ASSERT_TRUE(hit.has_value());
    EXPECT_GT(hit->time, 0.f);
    EXPECT_LE(hit->time, 1.f);
    EXPECT_NEAR(sgl::lengthSq(hit->normal), 1.f, 1e-4f);
    EXPECT_GT(hit->normal.x, 0.f);
    EXPECT_GT(hit->normal.y, 0.f);
    EXPECT_GT(std::fabs(hit->normal.x), 1e-3f);
    EXPECT_GT(std::fabs(hit->normal.y), 1e-3f);
}

TEST(SweepShould, reportTimeZeroWhenStartingInside)
{
    const sgl::Circle circle{.center = {247.f, 110.f}, .radius = 8.f};
    const auto hit = sgl::sweep(circle, {0.f, -50.f}, wideBrick);
    ASSERT_TRUE(hit.has_value());
    EXPECT_FLOAT_EQ(hit->time, 0.f);
    EXPECT_GT(hit->normal.y, 0.f);
}

TEST(SweepShould, returnNulloptForZeroVelocityOutside)
{
    const sgl::Circle circle{.center = {247.f, 200.f}, .radius = 8.f};
    EXPECT_FALSE(sgl::sweep(circle, {0.f, 0.f}, wideBrick).has_value());
}

TEST(SweepShould, notTunnelAtHighSpeed)
{
    // Delta is 10x brick width; must still register a left-face hit.
    const float width = wideBrick.size.x;
    const sgl::Circle circle{.center = {180.f, 94.f}, .radius = 8.f};
    const auto hit = sgl::sweep(circle, {10.f * width, 0.f}, wideBrick);
    ASSERT_TRUE(hit.has_value());
    EXPECT_GT(hit->time, 0.f);
    EXPECT_LE(hit->time, 1.f);
    EXPECT_NEAR(hit->normal.x, -1.f, 1e-5f);
    EXPECT_NEAR(hit->normal.y, 0.f, 1e-5f);
}

TEST(SweepShould, hitFaceWithAxisNormal)
{
    const float r = 8.f;
    const float cx = wideBrick.pos.x + wideBrick.size.x * 0.5f;
    const float cy = wideBrick.pos.y + wideBrick.size.y * 0.5f;

    {
        const sgl::Circle circle{.center = {wideBrick.pos.x - r - 20.f, cy}, .radius = r};
        const auto hit = sgl::sweep(circle, {40.f, 0.f}, wideBrick);
        ASSERT_TRUE(hit.has_value());
        EXPECT_NEAR(hit->normal.x, -1.f, 1e-5f);
        EXPECT_NEAR(hit->normal.y, 0.f, 1e-5f);
    }
    {
        const sgl::Circle circle{.center = {wideBrick.pos.x + wideBrick.size.x + r + 20.f, cy}, .radius = r};
        const auto hit = sgl::sweep(circle, {-40.f, 0.f}, wideBrick);
        ASSERT_TRUE(hit.has_value());
        EXPECT_NEAR(hit->normal.x, 1.f, 1e-5f);
        EXPECT_NEAR(hit->normal.y, 0.f, 1e-5f);
    }
    {
        const sgl::Circle circle{.center = {cx, wideBrick.pos.y - r - 20.f}, .radius = r};
        const auto hit = sgl::sweep(circle, {0.f, 40.f}, wideBrick);
        ASSERT_TRUE(hit.has_value());
        EXPECT_NEAR(hit->normal.x, 0.f, 1e-5f);
        EXPECT_NEAR(hit->normal.y, -1.f, 1e-5f);
    }
    {
        const sgl::Circle circle{.center = {cx, wideBrick.pos.y + wideBrick.size.y + r + 20.f}, .radius = r};
        const auto hit = sgl::sweep(circle, {0.f, -40.f}, wideBrick);
        ASSERT_TRUE(hit.has_value());
        EXPECT_NEAR(hit->normal.x, 0.f, 1e-5f);
        EXPECT_NEAR(hit->normal.y, 1.f, 1e-5f);
    }
}

TEST(ReflectShould, KeepsVelocityWhenMovingAway)
{
    const sgl::Vec2f velocity{0.f, 100.f};
    const sgl::Vec2f normal{0.f, 1.f};

    const sgl::Vec2f reflected = sgl::reflect(velocity, normal);
    EXPECT_FLOAT_EQ(reflected.x, velocity.x);
    EXPECT_FLOAT_EQ(reflected.y, velocity.y);
}

TEST(ReflectShould, BouncesWhenMovingIntoSurface)
{
    const sgl::Vec2f velocity{0.f, -100.f};
    const sgl::Vec2f normal{0.f, 1.f};

    const sgl::Vec2f reflected = sgl::reflect(velocity, normal);
    EXPECT_FLOAT_EQ(reflected.x, 0.f);
    EXPECT_FLOAT_EQ(reflected.y, 100.f);
}

TEST(ReflectShould, preserveSpeed)
{
    const sgl::Vec2f velocity{30.f, -40.f};
    const float speedSq = sgl::lengthSq(velocity);
    const sgl::Vec2f reflected = sgl::reflect(velocity, sgl::normalized(sgl::Vec2f{1.f, 1.f}).value());
    EXPECT_NEAR(sgl::lengthSq(reflected), speedSq, 1e-3f);
}
