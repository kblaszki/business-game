#include <algorithm>
#include <arkanoid/sim/Effects.hpp>
#include <arkanoid/sim/Levels.hpp>
#include <arkanoid/sim/Physics.hpp>
#include <arkanoid/sim/State.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <cmath>
#include <gtest/gtest.h>
#include <sgl/core/Time.hpp>
#include <sgl/core/Vec2.hpp>
#include <variant>

namespace
{

[[nodiscard]] float speedOf(const sgl::arkanoid::Ball& ball)
{
    return std::sqrt(sgl::lengthSq(ball.vel));
}

[[nodiscard]] sgl::Seconds remainingOf(const sgl::arkanoid::Effects& effects, std::size_t altIndex)
{
    for(const sgl::arkanoid::TimedEffect& effect: effects.active)
    {
        if(effect.index() == altIndex)
        {
            return std::visit([](const auto& timed) { return timed.remaining; }, effect);
        }
    }
    return sgl::Seconds{};
}

} // namespace

TEST(PowerUpTest, wideAndSlowStack)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);

    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::Wide);
    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::Slow);

    EXPECT_EQ(state.effects.active.size(), 2u);
    EXPECT_FLOAT_EQ(state.paddle.width, sgl::arkanoid::paddleWideWidth);
    EXPECT_FLOAT_EQ(sgl::arkanoid::paddleWidth(state.effects), sgl::arkanoid::paddleWideWidth);
    EXPECT_FLOAT_EQ(sgl::arkanoid::ballSpeedMultiplier(state.effects), sgl::arkanoid::slowFactor);
    EXPECT_TRUE(
        std::any_of(state.effects.active.begin(), state.effects.active.end(), [](const sgl::arkanoid::TimedEffect& e) {
            return std::holds_alternative<sgl::arkanoid::Wide>(e);
        }));
    EXPECT_TRUE(
        std::any_of(state.effects.active.begin(), state.effects.active.end(), [](const sgl::arkanoid::TimedEffect& e) {
            return std::holds_alternative<sgl::arkanoid::Slow>(e);
        }));
}

TEST(PowerUpTest, recatchRefreshesTimer)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);
    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::Wide);

    ASSERT_EQ(state.effects.active.size(), 1u);
    std::get<sgl::arkanoid::Wide>(state.effects.active.front()).remaining = sgl::Seconds{2.f};

    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::Wide);

    EXPECT_EQ(state.effects.active.size(), 1u);
    EXPECT_FLOAT_EQ(remainingOf(state.effects, 0).count(), sgl::arkanoid::effectDuration);
}

TEST(PowerUpTest, slowExpiryRestoresExactSpeed)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.balls = {sgl::arkanoid::Ball{
        .pos = {640.f, 360.f}, .vel = sgl::arkanoid::launchVelocity, .mode = sgl::arkanoid::BallMode::Live}};

    for(int i = 0; i < 50; ++i)
    {
        sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::Slow);
        ASSERT_EQ(state.effects.active.size(), 1u);
        std::get<sgl::arkanoid::Slow>(state.effects.active.front()).remaining = sgl::kTick;
        (void)sgl::arkanoid::step(state, {}, sgl::kTick);
        ASSERT_TRUE(state.effects.active.empty());
    }

    ASSERT_FALSE(state.balls.empty());
    EXPECT_FLOAT_EQ(speedOf(state.balls.front()), sgl::arkanoid::ballSpeed);
}

TEST(PowerUpTest, wideExpiryRestoresWidthAndClamps)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);
    state.paddle.x = sgl::arkanoid::designWidth - 50.f;
    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::Wide);

    EXPECT_FLOAT_EQ(state.paddle.width, sgl::arkanoid::paddleWideWidth);
    EXPECT_LE(state.paddle.x + state.paddle.width, sgl::arkanoid::designWidth);

    ASSERT_EQ(state.effects.active.size(), 1u);
    std::get<sgl::arkanoid::Wide>(state.effects.active.front()).remaining = sgl::kTick;
    (void)sgl::arkanoid::step(state, {}, sgl::kTick);

    EXPECT_TRUE(state.effects.active.empty());
    EXPECT_FLOAT_EQ(state.paddle.width, sgl::arkanoid::paddleNormalWidth);
    EXPECT_GE(state.paddle.x, 0.f);
    EXPECT_LE(state.paddle.x + state.paddle.width, sgl::arkanoid::designWidth);
}

TEST(PowerUpTest, multiBallDuringSlowKeepsSlowSpeed)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);
    state.balls.front().mode = sgl::arkanoid::BallMode::Live;
    state.balls.front().vel = sgl::arkanoid::launchVelocity;

    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::Slow);
    const float expected = sgl::arkanoid::ballSpeed * sgl::arkanoid::slowFactor;
    ASSERT_FLOAT_EQ(speedOf(state.balls.front()), expected);

    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::MultiBall);

    ASSERT_EQ(state.balls.size(), 3u);
    EXPECT_FLOAT_EQ(sgl::arkanoid::ballSpeedMultiplier(state.effects), sgl::arkanoid::slowFactor);
    EXPECT_FLOAT_EQ(speedOf(state.balls.front()), expected);
    EXPECT_FLOAT_EQ(state.balls[1].vel.y, state.balls[0].vel.y);
    EXPECT_FLOAT_EQ(state.balls[2].vel.y, state.balls[0].vel.y);
}

TEST(PowerUpTest, extraLifeDoesNotTouchTimers)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);
    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::Wide);
    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::Slow);

    ASSERT_EQ(state.effects.active.size(), 2u);
    std::get<sgl::arkanoid::Wide>(state.effects.active[0]).remaining = sgl::Seconds{3.f};
    std::get<sgl::arkanoid::Slow>(state.effects.active[1]).remaining = sgl::Seconds{5.f};
    const std::uint32_t livesBefore = state.lives;

    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::ExtraLife);

    EXPECT_EQ(state.lives, livesBefore + 1);
    EXPECT_EQ(state.effects.active.size(), 2u);
    EXPECT_FLOAT_EQ(remainingOf(state.effects, 0).count(), 3.f);
    EXPECT_FLOAT_EQ(remainingOf(state.effects, 1).count(), 5.f);
}
