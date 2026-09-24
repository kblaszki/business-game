#include <arkanoid/app/SceneRender.hpp>
#include <arkanoid/sim/Levels.hpp>
#include <arkanoid/sim/Tuning.hpp>

#include <eng/render/DrawCommand.hpp>
#include <eng/render/RenderQueue.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <variant>

namespace
{

arkanoid::TextureIds dummyTextures()
{
    return arkanoid::TextureIds{
        .background = eng::TextureId{1},
        .brick = eng::TextureId{2},
        .paddle = eng::TextureId{3},
        .ball = eng::TextureId{4},
        .capsule =
            {
                eng::TextureId{5},
                eng::TextureId{6},
                eng::TextureId{7},
                eng::TextureId{8},
            },
    };
}

} // namespace

TEST(SceneRenderShould, stage2CommandCountMatchesEntities)
{
    arkanoid::State state = arkanoid::makeState(arkanoid::StageId::Stage2);
    eng::RenderQueue queue;
    arkanoid::renderState(state, dummyTextures(), queue);

    // 1 background + 30 bricks + 1 paddle + 1 ball
    EXPECT_EQ(queue.size(), 33u);
}

TEST(SceneRenderShould, deadBrickIsNotDrawn)
{
    arkanoid::State state = arkanoid::makeState(arkanoid::StageId::Stage2);
    eng::RenderQueue queue;
    const arkanoid::TextureIds textures = dummyTextures();
    arkanoid::renderState(state, textures, queue);
    const std::size_t aliveCount = queue.size();

    ASSERT_FALSE(state.bricks.empty());
    state.bricks.front().alive = false;
    queue.clear();
    arkanoid::renderState(state, textures, queue);

    EXPECT_EQ(queue.size(), aliveCount - 1u);
}

TEST(SceneRenderShould, sortedLayersAreBackgroundWorldActors)
{
    arkanoid::State state = arkanoid::makeState(arkanoid::StageId::Stage2);
    eng::RenderQueue queue;
    arkanoid::renderState(state, dummyTextures(), queue);

    const auto sorted = queue.sorted();
    ASSERT_GE(sorted.size(), 3u);
    EXPECT_EQ(sorted.front().key.layer, eng::Layer::Background);

    bool sawWorld = false;
    bool sawActors = false;
    eng::Layer prev = eng::Layer::Background;
    for(const eng::Entry& entry : sorted)
    {
        EXPECT_GE(static_cast<std::uint8_t>(entry.key.layer), static_cast<std::uint8_t>(prev));
        if(entry.key.layer == eng::Layer::World)
        {
            sawWorld = true;
        }
        if(entry.key.layer == eng::Layer::Actors)
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
    arkanoid::State state = arkanoid::makeState(arkanoid::StageId::Stage2);
    state.paddle.width = 200.f;
    eng::RenderQueue queue;
    const arkanoid::TextureIds textures = dummyTextures();
    arkanoid::renderState(state, textures, queue);

    const eng::SpriteCmd* paddleCmd = nullptr;
    for(const eng::Entry& entry : queue.sorted())
    {
        if(const auto* sprite = std::get_if<eng::SpriteCmd>(&entry.command);
           sprite != nullptr && sprite->texture == textures.paddle)
        {
            paddleCmd = sprite;
            break;
        }
    }
    ASSERT_NE(paddleCmd, nullptr);
    EXPECT_NEAR(paddleCmd->scale.x, 200.f / arkanoid::paddleWidth, 1e-5f);
    EXPECT_FLOAT_EQ(paddleCmd->scale.y, 1.f);
}
