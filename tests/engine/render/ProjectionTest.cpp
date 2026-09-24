#include <eng/render/Camera.hpp>
#include <eng/render/Projection.hpp>
#include <gtest/gtest.h>

static_assert(eng::Projection<eng::Orthographic> && eng::Projection<eng::Isometric>);

TEST(ProjectionTest, IsometricProjectsUnitX)
{
    const eng::Isometric iso{.tileW = 64.f, .tileH = 32.f};
    const eng::Projected p = iso.project(eng::Vec3f{1.f, 0.f, 0.f});

    EXPECT_FLOAT_EQ(p.screen.x, 32.f);
    EXPECT_FLOAT_EQ(p.screen.y, 16.f);
    EXPECT_FLOAT_EQ(p.depth, 1.f);
}

TEST(ProjectionTest, LargerXyGivesLargerDepth)
{
    const eng::Isometric iso{.tileW = 64.f, .tileH = 32.f};
    const eng::Projected a = iso.project(eng::Vec3f{1.f, 0.f, 0.f});
    const eng::Projected b = iso.project(eng::Vec3f{1.f, 1.f, 0.f});

    EXPECT_GT(b.depth, a.depth);
}

TEST(ProjectionTest, OrthographicUsesXyAndZ)
{
    const eng::Orthographic ortho{};
    const eng::Projected p = ortho.project(eng::Vec3f{3.f, 4.f, 5.f});

    EXPECT_FLOAT_EQ(p.screen.x, 3.f);
    EXPECT_FLOAT_EQ(p.screen.y, 4.f);
    EXPECT_FLOAT_EQ(p.depth, 5.f);
}

TEST(CameraTest, ApplyOffsetsAndZooms)
{
    const eng::Camera camera{.offset = {10.f, 20.f}, .zoom = 2.f};
    const eng::Vec2f out = camera.apply({15.f, 30.f});

    EXPECT_FLOAT_EQ(out.x, 10.f);
    EXPECT_FLOAT_EQ(out.y, 20.f);
}
