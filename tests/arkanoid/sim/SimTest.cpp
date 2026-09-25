#include <algorithm>
#include <arkanoid/sim/Events.hpp>
#include <arkanoid/sim/Levels.hpp>
#include <arkanoid/sim/Physics.hpp>
#include <arkanoid/sim/State.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <gtest/gtest.h>
#include <sgl/core/Time.hpp>
#include <variant>

namespace
{

[[nodiscard]] bool hasEvent(const std::vector<sgl::arkanoid::SimEvent>& events, auto pred)
{
    return std::any_of(events.begin(), events.end(), pred);
}

} // namespace

TEST(SimShould, clampPaddleInsideDesignWidth)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);
    for(int i = 0; i < 60; ++i)
    {
        (void)sgl::arkanoid::step(state, {.paddleAxis = 1.f}, sgl::kTick);
    }

    EXPECT_GE(state.paddle.x, 0.f);
    EXPECT_LE(state.paddle.x + state.paddle.width, sgl::arkanoid::designWidth);
}

TEST(SimShould, bounceBallOffLeftWall)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);
    state.bricks.clear();
    state.balls = {
        sgl::arkanoid::Ball{.pos = {2.f, 300.f}, .vel = {-240.f, 0.f}, .mode = sgl::arkanoid::BallMode::Live}};

    (void)sgl::arkanoid::step(state, {}, sgl::kTick);

    ASSERT_FALSE(state.balls.empty());
    EXPECT_GE(state.balls.front().pos.x, 0.f);
    EXPECT_GT(state.balls.front().vel.x, 0.f);
}

TEST(SimShould, breakBrickRaisesScoreAndBrickDestroyed)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.bricks.push_back(sgl::arkanoid::Brick{.box = {{200.f, 80.f}, {110.f, 28.f}}, .alive = true});
    state.balls = {
        sgl::arkanoid::Ball{.pos = {247.f, 96.f}, .vel = {0.f, -300.f}, .mode = sgl::arkanoid::BallMode::Live}};

    const auto events = sgl::arkanoid::step(state, {}, sgl::kTick);

    EXPECT_FALSE(state.bricks.front().alive);
    EXPECT_EQ(state.score, 10u);
    EXPECT_TRUE(hasEvent(events, [](const sgl::arkanoid::SimEvent& e) {
        return std::holds_alternative<sgl::arkanoid::BrickDestroyed>(e);
    }));
}

TEST(SimShould, bounceDownWhenHittingBrickFromBelow)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.bricks.push_back(sgl::arkanoid::Brick{.box = {{200.f, 80.f}, {110.f, 28.f}}, .alive = true});
    state.balls = {
        sgl::arkanoid::Ball{.pos = {247.f, 100.f}, .vel = {0.f, -300.f}, .mode = sgl::arkanoid::BallMode::Live}};

    (void)sgl::arkanoid::step(state, {}, sgl::kTick);

    EXPECT_FALSE(state.bricks.front().alive);
    EXPECT_GT(state.balls.front().vel.y, 0.f);
}

TEST(SimShould, loseLifeAndRestickWhenOnlyBallExitsBottom)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);
    state.bricks.clear();
    state.balls = {
        sgl::arkanoid::Ball{.pos = {400.f, 730.f}, .vel = {0.f, 240.f}, .mode = sgl::arkanoid::BallMode::Live}};

    const auto events = sgl::arkanoid::step(state, {}, sgl::kTick);

    EXPECT_EQ(state.lives, 2u);
    ASSERT_EQ(state.balls.size(), 1u);
    EXPECT_EQ(state.balls.front().mode, sgl::arkanoid::BallMode::Stuck);
    EXPECT_FALSE(state.over);
    EXPECT_TRUE(hasEvent(
        events, [](const sgl::arkanoid::SimEvent& e) { return std::holds_alternative<sgl::arkanoid::LifeLost>(e); }));
}

TEST(SimShould, emitStageClearedWhenLastBrickDies)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.bricks.push_back(sgl::arkanoid::Brick{.box = {{200.f, 80.f}, {110.f, 28.f}}, .alive = true});
    state.balls = {
        sgl::arkanoid::Ball{.pos = {247.f, 96.f}, .vel = {0.f, -300.f}, .mode = sgl::arkanoid::BallMode::Live}};

    const auto events = sgl::arkanoid::step(state, {}, sgl::kTick);

    EXPECT_TRUE(state.cleared);
    EXPECT_TRUE(hasEvent(events, [](const sgl::arkanoid::SimEvent& e) {
        return std::holds_alternative<sgl::arkanoid::StageCleared>(e);
    }));
}

TEST(SimShould, emptyBricksDoNotSetCleared)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.balls = {sgl::arkanoid::Ball{.pos = {640.f, 664.f}, .mode = sgl::arkanoid::BallMode::Stuck}};

    (void)sgl::arkanoid::step(state, {}, sgl::kTick);

    EXPECT_TRUE(state.bricks.empty());
    EXPECT_FALSE(state.cleared);
}

TEST(SimShould, makeStateBrickCounts)
{
    EXPECT_EQ(sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1).bricks.size(), 60u);
    EXPECT_EQ(sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage2).bricks.size(), 30u);
    EXPECT_EQ(sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage3).bricks.size(), 28u);
}

TEST(SimShould, applyPowerUpWideAndExtraLife)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);
    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::Wide);
    EXPECT_GT(state.paddle.width, 120.f);

    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::ExtraLife);
    EXPECT_EQ(state.lives, 4u);
}

TEST(SimShould, missedCapsuleDisappears)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);
    state.balls.clear();
    state.balls.push_back(sgl::arkanoid::Ball{.pos = {640.f, 664.f}, .mode = sgl::arkanoid::BallMode::Stuck});
    state.capsules.push_back(sgl::arkanoid::Capsule{.pos = {200.f, 80.f}, .kind = sgl::arkanoid::PowerUpKind::Wide});

    for(int i = 0; i < 600 && !state.capsules.empty(); ++i)
    {
        (void)sgl::arkanoid::step(state, {}, sgl::kTick);
    }

    EXPECT_TRUE(state.capsules.empty());
}

TEST(SimShould, slowThenExpireRestoresSpeed)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.balls = {
        sgl::arkanoid::Ball{.pos = {640.f, 360.f}, .vel = {160.f, -360.f}, .mode = sgl::arkanoid::BallMode::Live}};

    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::Slow);
    ASSERT_NEAR(state.balls.front().vel.x, 160.f * sgl::arkanoid::slowFactor, 1e-3f);
    ASSERT_NEAR(state.balls.front().vel.y, -360.f * sgl::arkanoid::slowFactor, 1e-3f);

    state.effects.remaining = sgl::kTick;
    (void)sgl::arkanoid::step(state, {}, sgl::kTick);

    ASSERT_FALSE(state.balls.empty());
    EXPECT_FALSE(state.effects.slowActive);
    EXPECT_NEAR(state.balls.front().vel.x, 160.f, 1e-3f);
    EXPECT_NEAR(state.balls.front().vel.y, -360.f, 1e-3f);
}

TEST(SimShould, multiballCountBecomesThree)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);
    sgl::arkanoid::applyPowerUp(state, sgl::arkanoid::PowerUpKind::MultiBall);
    EXPECT_EQ(state.balls.size(), 3u);
}

TEST(SimShould, tunnelingBallDestroysBrick)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.bricks.push_back(sgl::arkanoid::Brick{.box = {{200.f, 80.f}, {110.f, 28.f}}, .alive = true});
    // Far below the brick: one 1200 px/s × 0.1s step tunnels through without endpoint overlap.
    state.balls = {
        sgl::arkanoid::Ball{.pos = {247.f, 140.f}, .vel = {0.f, -1200.f}, .mode = sgl::arkanoid::BallMode::Live}};

    const auto events = sgl::arkanoid::step(state, {}, sgl::Seconds{0.1f});

    EXPECT_FALSE(state.bricks.front().alive);
    EXPECT_TRUE(hasEvent(events, [](const sgl::arkanoid::SimEvent& e) {
        return std::holds_alternative<sgl::arkanoid::BrickDestroyed>(e);
    }));
}
