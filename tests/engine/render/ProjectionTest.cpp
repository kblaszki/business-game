#include <gtest/gtest.h>
#include <sgl/render/Camera.hpp>
#include <sgl/render/Projection.hpp>

static_assert(sgl::Projection<sgl::Orthographic> && sgl::Projection<sgl::Isometric>);

TEST(ProjectionTest, IsometricProjectsUnitX)
{
    const sgl::Isometric iso{.tileW = 64.f, .tileH = 32.f};
    const sgl::Projected p = iso.project(sgl::Vec3f{1.f, 0.f, 0.f});

    EXPECT_FLOAT_EQ(p.screen.x, 32.f);
    EXPECT_FLOAT_EQ(p.screen.y, 16.f);
    EXPECT_FLOAT_EQ(p.depth, 1.f);
}

TEST(ProjectionTest, LargerXyGivesLargerDepth)
{
    const sgl::Isometric iso{.tileW = 64.f, .tileH = 32.f};
    const sgl::Projected a = iso.project(sgl::Vec3f{1.f, 0.f, 0.f});
    const sgl::Projected b = iso.project(sgl::Vec3f{1.f, 1.f, 0.f});

    EXPECT_GT(b.depth, a.depth);
}

TEST(ProjectionTest, OrthographicUsesXyAndZ)
{
    const sgl::Orthographic ortho{};
    const sgl::Projected p = ortho.project(sgl::Vec3f{3.f, 4.f, 5.f});

    EXPECT_FLOAT_EQ(p.screen.x, 3.f);
    EXPECT_FLOAT_EQ(p.screen.y, 4.f);
    EXPECT_FLOAT_EQ(p.depth, 5.f);
}

TEST(CameraTest, ApplyOffsetsAndZooms)
{
    const sgl::Camera camera{.offset = {10.f, 20.f}, .zoom = 2.f};
    const sgl::Vec2f out = camera.apply({15.f, 30.f});

    EXPECT_FLOAT_EQ(out.x, 10.f);
    EXPECT_FLOAT_EQ(out.y, 20.f);
}
