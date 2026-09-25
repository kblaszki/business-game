#include <arkanoid/app/SceneRender.hpp>
#include <arkanoid/sim/Levels.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <cstdint>
#include <gtest/gtest.h>
#include <sgl/render/DrawCommand.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <variant>

namespace
{

sgl::arkanoid::TextureIds dummyTextures()
{
    return sgl::arkanoid::TextureIds{
        .background = sgl::TextureId{1},
        .brick = sgl::TextureId{2},
        .paddle = sgl::TextureId{3},
        .ball = sgl::TextureId{4},
        .capsule =
            {
                sgl::TextureId{5},
                sgl::TextureId{6},
                sgl::TextureId{7},
                sgl::TextureId{8},
            },
    };
}

} // namespace

TEST(SceneRenderShould, stage2CommandCountMatchesEntities)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage2);
    sgl::RenderQueue queue;
    sgl::arkanoid::renderState(state, dummyTextures(), queue);

    // 1 background + 30 bricks + 1 paddle + 1 ball
    EXPECT_EQ(queue.size(), 33u);
}

TEST(SceneRenderShould, deadBrickIsNotDrawn)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage2);
    sgl::RenderQueue queue;
    const sgl::arkanoid::TextureIds textures = dummyTextures();
    sgl::arkanoid::renderState(state, textures, queue);
    const std::size_t aliveCount = queue.size();

    ASSERT_FALSE(state.bricks.empty());
    state.bricks.front().alive = false;
    queue.clear();
    sgl::arkanoid::renderState(state, textures, queue);

    EXPECT_EQ(queue.size(), aliveCount - 1u);
}

TEST(SceneRenderShould, sortedLayersAreBackgroundWorldActors)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage2);
    sgl::RenderQueue queue;
    sgl::arkanoid::renderState(state, dummyTextures(), queue);

    const auto sorted = queue.sorted();
    ASSERT_GE(sorted.size(), 3u);
    EXPECT_EQ(sorted.front().key.layer, sgl::Layer::Background);

    bool sawWorld = false;
    bool sawActors = false;
    sgl::Layer prev = sgl::Layer::Background;
    for(const sgl::Entry& entry: sorted)
    {
        EXPECT_GE(static_cast<std::uint8_t>(entry.key.layer), static_cast<std::uint8_t>(prev));
        if(entry.key.layer == sgl::Layer::World)
        {
            sawWorld = true;
        }
        if(entry.key.layer == sgl::Layer::Actors)
        {
            sawActors = true;
        }
        prev = entry.key.layer;
    }
    EXPECT_TRUE(sawWorld);
    EXPECT_TRUE(sawActors);
}

TEST(SceneRenderShould, paddleScaleXMatchesWidthOverBase)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage2);
    state.paddle.width = 200.f;
    sgl::RenderQueue queue;
    const sgl::arkanoid::TextureIds textures = dummyTextures();
    sgl::arkanoid::renderState(state, textures, queue);

    const sgl::SpriteCmd* paddleCmd = nullptr;
    for(const sgl::Entry& entry: queue.sorted())
    {
        if(const auto* sprite = std::get_if<sgl::SpriteCmd>(&entry.command);
           sprite != nullptr && sprite->texture == textures.paddle)
        {
            paddleCmd = sprite;
            break;
        }
    }
    ASSERT_NE(paddleCmd, nullptr);
    EXPECT_NEAR(paddleCmd->scale.x, 200.f / sgl::arkanoid::paddleWidth, 1e-5f);
    EXPECT_FLOAT_EQ(paddleCmd->scale.y, 1.f);
}
