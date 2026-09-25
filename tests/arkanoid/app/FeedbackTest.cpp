#include <mocks/AudioSpy.hpp>

#include <arkanoid/app/Feedback.hpp>
#include <arkanoid/sim/Events.hpp>
#include <arkanoid/sim/State.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <gtest/gtest.h>
#include <sgl/core/Color.hpp>
#include <sgl/core/Random.hpp>
#include <sgl/render/DrawCommand.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <span>
#include <variant>

namespace
{

sgl::arkanoid::SoundIds makeIds()
{
    return sgl::arkanoid::SoundIds{
        .paddle = sgl::SoundId{.id = 1},
        .brick = sgl::SoundId{.id = 2},
        .wall = sgl::SoundId{.id = 3},
        .launch = sgl::SoundId{.id = 4},
        .powerUp = sgl::SoundId{.id = 5},
        .ballLost = sgl::SoundId{.id = 6},
        .lifeLost = sgl::SoundId{.id = 7},
        .stageClear = sgl::SoundId{.id = 8},
        .gameOver = sgl::SoundId{.id = 9},
    };
}

[[nodiscard]] sgl::arkanoid::State stateWithBrick(sgl::Color tint)
{
    sgl::arkanoid::State state{};
    state.bricks.push_back(sgl::arkanoid::Brick{
        .box = {.pos = {100.f, 200.f}, .size = {sgl::arkanoid::brickWidth, sgl::arkanoid::brickHeight}},
        .tint = tint,
        .alive = false,
    });
    return state;
}

} // namespace

TEST(FeedbackTest, brickDestroyedPlaysBrickAndEmitsTwelve)
{
    sgl::AudioSpy audio;
    const auto ids = makeIds();
    sgl::arkanoid::Feedback feedback{audio, ids, sgl::Pcg32{1}};
    const sgl::arkanoid::State state = stateWithBrick({200, 100, 50, 255});

    const sgl::arkanoid::SimEvent event = sgl::arkanoid::BrickDestroyed{.index = 0};
    feedback.onEvents(std::span{&event, 1}, state);

    ASSERT_EQ(audio.plays.size(), 1u);
    EXPECT_EQ(audio.plays[0].id, ids.brick);
    EXPECT_EQ(feedback.particleCount(), sgl::arkanoid::Feedback::kParticlesPerBrick);
}

TEST(FeedbackTest, paddleHitPlaysPaddle)
{
    sgl::AudioSpy audio;
    const auto ids = makeIds();
    sgl::arkanoid::Feedback feedback{audio, ids, sgl::Pcg32{2}};
    const sgl::arkanoid::State state{};

    const sgl::arkanoid::SimEvent event = sgl::arkanoid::PaddleHit{};
    feedback.onEvents(std::span{&event, 1}, state);

    ASSERT_EQ(audio.plays.size(), 1u);
    EXPECT_EQ(audio.plays[0].id, ids.paddle);
    EXPECT_EQ(feedback.particleCount(), 0u);
}

TEST(FeedbackTest, lifeLostPlaysLifeLost)
{
    sgl::AudioSpy audio;
    const auto ids = makeIds();
    sgl::arkanoid::Feedback feedback{audio, ids, sgl::Pcg32{3}};
    const sgl::arkanoid::State state{};

    const sgl::arkanoid::SimEvent event = sgl::arkanoid::LifeLost{};
    feedback.onEvents(std::span{&event, 1}, state);

    ASSERT_EQ(audio.plays.size(), 1u);
    EXPECT_EQ(audio.plays[0].id, ids.lifeLost);
}

TEST(FeedbackTest, noEventsNoSound)
{
    sgl::AudioSpy audio;
    const auto ids = makeIds();
    sgl::arkanoid::Feedback feedback{audio, ids, sgl::Pcg32{4}};
    const sgl::arkanoid::State state{};

    feedback.onEvents({}, state);

    EXPECT_TRUE(audio.plays.empty());
    EXPECT_EQ(feedback.particleCount(), 0u);
}

TEST(FeedbackTest, particlesUseBrickColor)
{
    sgl::AudioSpy audio;
    const auto ids = makeIds();
    sgl::arkanoid::Feedback feedback{audio, ids, sgl::Pcg32{5}};
    const sgl::Color tint{12, 34, 56, 200};
    const sgl::arkanoid::State state = stateWithBrick(tint);

    const sgl::arkanoid::SimEvent event = sgl::arkanoid::BrickDestroyed{.index = 0};
    feedback.onEvents(std::span{&event, 1}, state);

    sgl::RenderQueue queue;
    feedback.render(queue);
    ASSERT_EQ(queue.size(), sgl::arkanoid::Feedback::kParticlesPerBrick);
    for(const sgl::Entry& entry: queue.sorted())
    {
        const auto& cmd = std::get<sgl::RectCmd>(entry.command);
        EXPECT_EQ(cmd.fill.r, tint.r);
        EXPECT_EQ(cmd.fill.g, tint.g);
        EXPECT_EQ(cmd.fill.b, tint.b);
    }
}
