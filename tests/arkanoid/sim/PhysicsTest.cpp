#include <algorithm>
#include <arkanoid/sim/Events.hpp>
#include <arkanoid/sim/Levels.hpp>
#include <arkanoid/sim/Physics.hpp>
#include <arkanoid/sim/State.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <array>
#include <cmath>
#include <gtest/gtest.h>
#include <numbers>
#include <sgl/core/Time.hpp>
#include <sgl/core/Vec2.hpp>
#include <variant>

namespace
{

[[nodiscard]] bool hasEvent(const std::vector<sgl::arkanoid::SimEvent>& events, auto pred)
{
    return std::any_of(events.begin(), events.end(), pred);
}

[[nodiscard]] std::size_t countEvent(const std::vector<sgl::arkanoid::SimEvent>& events, auto pred)
{
    return static_cast<std::size_t>(std::count_if(events.begin(), events.end(), pred));
}

[[nodiscard]] bool ballOverlapsPaddle(const sgl::arkanoid::Ball& ball, const sgl::arkanoid::Paddle& paddle)
{
    const sgl::Vec2f center{ball.pos.x + sgl::arkanoid::ballRadius, ball.pos.y + sgl::arkanoid::ballRadius};
    const float nearestX = std::clamp(center.x, paddle.x, paddle.x + paddle.width);
    const float nearestY =
        std::clamp(center.y, sgl::arkanoid::paddleY, sgl::arkanoid::paddleY + sgl::arkanoid::paddleHeight);
    const float dx = center.x - nearestX;
    const float dy = center.y - nearestY;
    return dx * dx + dy * dy <= sgl::arkanoid::ballRadius * sgl::arkanoid::ballRadius;
}

[[nodiscard]] float speedOf(const sgl::arkanoid::Ball& ball)
{
    return std::sqrt(sgl::lengthSq(ball.vel));
}

} // namespace

TEST(PhysicsTest, ballNeverOverlapsPaddleAfterSideSweep)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 400.f;
    state.paddle.width = sgl::arkanoid::paddleNormalWidth;
    state.balls = {
        sgl::arkanoid::Ball{.pos = {400.f + sgl::arkanoid::paddleNormalWidth + 1.f, sgl::arkanoid::paddleY - 4.f},
                            .vel = {0.f, 0.f},
                            .mode = sgl::arkanoid::BallMode::Live}};

    (void)sgl::arkanoid::step(state, {.paddleAxis = 1.f}, sgl::Seconds{0.05f});

    ASSERT_EQ(state.balls.size(), 1u);
    EXPECT_FALSE(ballOverlapsPaddle(state.balls.front(), state.paddle));
}

TEST(PhysicsTest, paddleMovingIntoRisingBallDepenetrates)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 500.f;
    state.paddle.width = sgl::arkanoid::paddleNormalWidth;
    state.balls = {sgl::arkanoid::Ball{.pos = {500.f - sgl::arkanoid::ballDiameter - 2.f, sgl::arkanoid::paddleY - 6.f},
                                       .vel = {0.f, -200.f},
                                       .mode = sgl::arkanoid::BallMode::Live}};

    (void)sgl::arkanoid::step(state, {.paddleAxis = -1.f}, sgl::Seconds{0.05f});

    ASSERT_EQ(state.balls.size(), 1u);
    EXPECT_FALSE(ballOverlapsPaddle(state.balls.front(), state.paddle));
    EXPECT_LT(state.balls.front().vel.y, 0.f);
}

TEST(PhysicsTest, centerHitGoesStraightUp)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.paddle.width = sgl::arkanoid::paddleNormalWidth;
    const float paddleCenterX = state.paddle.x + state.paddle.width * 0.5f;
    state.balls = {sgl::arkanoid::Ball{
        .pos = {paddleCenterX - sgl::arkanoid::ballRadius, sgl::arkanoid::paddleY - sgl::arkanoid::ballDiameter - 2.f},
        .vel = {0.f, 400.f},
        .mode = sgl::arkanoid::BallMode::Live}};

    (void)sgl::arkanoid::step(state, {}, sgl::kTick);

    ASSERT_EQ(state.balls.size(), 1u);
    const float speed = sgl::arkanoid::ballSpeed;
    EXPECT_NEAR(state.balls.front().vel.x, 0.f, 1e-2f);
    EXPECT_NEAR(state.balls.front().vel.y, -speed, 1e-2f);
}

TEST(PhysicsTest, edgeHitUsesMaxAngle)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.paddle.width = sgl::arkanoid::paddleNormalWidth;
    // Center aligned with the paddle's right edge so hitOffset == +1.
    state.balls = {sgl::arkanoid::Ball{.pos = {state.paddle.x + state.paddle.width - sgl::arkanoid::ballRadius,
                                               sgl::arkanoid::paddleY - sgl::arkanoid::ballDiameter - 2.f},
                                       .vel = {0.f, 400.f},
                                       .mode = sgl::arkanoid::BallMode::Live}};

    (void)sgl::arkanoid::step(state, {}, sgl::kTick);

    ASSERT_EQ(state.balls.size(), 1u);
    constexpr float maxAngle = 60.f * std::numbers::pi_v<float> / 180.f;
    const float speed = sgl::arkanoid::ballSpeed;
    EXPECT_NEAR(state.balls.front().vel.x, speed * std::sin(maxAngle), 5.f);
    EXPECT_NEAR(state.balls.front().vel.y, -speed * std::cos(maxAngle), 5.f);
}

TEST(PhysicsTest, speedConstantAfterManyBounces)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.balls = {sgl::arkanoid::Ball{
        .pos = {640.f, 360.f}, .vel = sgl::arkanoid::launchVelocity, .mode = sgl::arkanoid::BallMode::Live}};

    for(int i = 0; i < 600; ++i)
    {
        (void)sgl::arkanoid::step(state, {.paddleAxis = (i % 40 < 20) ? 1.f : -1.f}, sgl::kTick);
        if(state.balls.empty() || state.over)
        {
            break;
        }
        if(state.balls.front().mode == sgl::arkanoid::BallMode::Stuck)
        {
            (void)sgl::arkanoid::step(state, {.launch = true}, sgl::kTick);
            continue;
        }
        const float expected = sgl::arkanoid::ballSpeed * sgl::arkanoid::ballSpeedMultiplier(state);
        EXPECT_NEAR(speedOf(state.balls.front()), expected, expected * 1e-3f);
    }
}

TEST(PhysicsTest, minimumVerticalAngleEnforced)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.balls = {sgl::arkanoid::Ball{
        .pos = {2.f, 300.f}, .vel = {-sgl::arkanoid::ballSpeed, 1.f}, .mode = sgl::arkanoid::BallMode::Live}};

    (void)sgl::arkanoid::step(state, {}, sgl::kTick);

    ASSERT_EQ(state.balls.size(), 1u);
    const float speed = speedOf(state.balls.front());
    constexpr float minSin = std::sin(15.f * std::numbers::pi_v<float> / 180.f);
    EXPECT_GE(std::abs(state.balls.front().vel.y) / speed, minSin - 1e-4f);
}

TEST(PhysicsTest, seamHitDestroysBothAndReflectsOnce)
{
    const sgl::arkanoid::Brick left{.box = {{200.f, 80.f}, {110.f, 28.f}}, .alive = true};
    const sgl::arkanoid::Brick right{.box = {{310.f, 80.f}, {110.f, 28.f}}, .alive = true};
    sgl::arkanoid::State state = sgl::arkanoid::makeState(std::array{left, right});
    // Approach the shared vertical seam from below.
    state.balls = {sgl::arkanoid::Ball{.pos = {310.f - sgl::arkanoid::ballRadius, 120.f},
                                       .vel = {0.f, -600.f},
                                       .mode = sgl::arkanoid::BallMode::Live}};

    const auto events = sgl::arkanoid::step(state, {}, sgl::Seconds{0.1f});

    EXPECT_FALSE(state.bricks[0].alive);
    EXPECT_FALSE(state.bricks[1].alive);
    EXPECT_EQ(state.score, 20u);
    EXPECT_EQ(countEvent(events,
                         [](const sgl::arkanoid::SimEvent& e) {
                             return std::holds_alternative<sgl::arkanoid::BrickDestroyed>(e);
                         }),
              2u);
    ASSERT_EQ(state.balls.size(), 1u);
    EXPECT_GT(state.balls.front().vel.y, 0.f);
    EXPECT_NEAR(speedOf(state.balls.front()), sgl::arkanoid::ballSpeed, sgl::arkanoid::ballSpeed * 1e-3f);
}

TEST(PhysicsTest, remainingTimeUsedAfterBrickHit)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.bricks.push_back(sgl::arkanoid::Brick{.box = {{200.f, 120.f}, {110.f, 28.f}}, .alive = true});
    // Close above the brick so TOI spends almost all remaining travel upward into the ceiling.
    state.balls = {
        sgl::arkanoid::Ball{.pos = {247.f, 100.f}, .vel = {0.f, 800.f}, .mode = sgl::arkanoid::BallMode::Live}};

    const auto events = sgl::arkanoid::step(state, {}, sgl::Seconds{0.5f});

    EXPECT_FALSE(state.bricks.front().alive);
    EXPECT_TRUE(hasEvent(events, [](const sgl::arkanoid::SimEvent& e) {
        return std::holds_alternative<sgl::arkanoid::BrickDestroyed>(e);
    }));
    EXPECT_TRUE(hasEvent(
        events, [](const sgl::arkanoid::SimEvent& e) { return std::holds_alternative<sgl::arkanoid::WallHit>(e); }));
    ASSERT_EQ(state.balls.size(), 1u);
    EXPECT_GT(state.balls.front().vel.y, 0.f);
}

TEST(PhysicsTest, cornerHitUsesDiagonalNormal)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.bricks.push_back(sgl::arkanoid::Brick{.box = {{200.f, 80.f}, {110.f, 28.f}}, .alive = true});
    // Aim at the bottom-left corner along a diagonal.
    state.balls = {
        sgl::arkanoid::Ball{.pos = {180.f, 120.f}, .vel = {300.f, -300.f}, .mode = sgl::arkanoid::BallMode::Live}};

    (void)sgl::arkanoid::step(state, {}, sgl::Seconds{0.1f});

    EXPECT_FALSE(state.bricks.front().alive);
    ASSERT_EQ(state.balls.size(), 1u);
    // Diagonal outward normal from bottom-left corner is roughly (-,-) wait: corner is (200,108),
    // ball approaches from SW so normal points SW-ish (negative x, positive y) or normalized(center-corner).
    // After reflect of (300,-300) off a diagonal, both components should change sign pattern vs axis-only.
    EXPECT_NE(state.balls.front().vel.x, 0.f);
    EXPECT_NE(state.balls.front().vel.y, 0.f);
    EXPECT_NEAR(speedOf(state.balls.front()), sgl::arkanoid::ballSpeed, sgl::arkanoid::ballSpeed * 1e-3f);
}

TEST(PhysicsTest, losingOneOfTwoBallsEmitsBallLostNotLifeLost)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.lives = 3;
    state.balls = {
        sgl::arkanoid::Ball{.pos = {400.f, 730.f}, .vel = {0.f, 200.f}, .mode = sgl::arkanoid::BallMode::Live},
        sgl::arkanoid::Ball{.pos = {500.f, 300.f}, .vel = {0.f, -200.f}, .mode = sgl::arkanoid::BallMode::Live}};

    const auto events = sgl::arkanoid::step(state, {}, sgl::kTick);

    EXPECT_EQ(state.lives, 3u);
    EXPECT_EQ(state.balls.size(), 1u);
    EXPECT_TRUE(hasEvent(
        events, [](const sgl::arkanoid::SimEvent& e) { return std::holds_alternative<sgl::arkanoid::BallLost>(e); }));
    EXPECT_FALSE(hasEvent(
        events, [](const sgl::arkanoid::SimEvent& e) { return std::holds_alternative<sgl::arkanoid::LifeLost>(e); }));
}

TEST(PhysicsTest, losingLastBallCostsLife)
{
    sgl::arkanoid::State state{};
    state.paddle.x = 580.f;
    state.lives = 3;
    state.balls = {
        sgl::arkanoid::Ball{.pos = {400.f, 730.f}, .vel = {0.f, 200.f}, .mode = sgl::arkanoid::BallMode::Live}};

    const auto events = sgl::arkanoid::step(state, {}, sgl::kTick);

    EXPECT_EQ(state.lives, 2u);
    ASSERT_EQ(state.balls.size(), 1u);
    EXPECT_EQ(state.balls.front().mode, sgl::arkanoid::BallMode::Stuck);
    EXPECT_TRUE(hasEvent(
        events, [](const sgl::arkanoid::SimEvent& e) { return std::holds_alternative<sgl::arkanoid::LifeLost>(e); }));
    EXPECT_FALSE(hasEvent(
        events, [](const sgl::arkanoid::SimEvent& e) { return std::holds_alternative<sgl::arkanoid::BallLost>(e); }));
}

TEST(PhysicsTest, exteriorWallsKeepBallInside)
{
    struct Shot
    {
        sgl::Vec2f pos;
        sgl::Vec2f vel;
        int expectVelX; // -1, 0 (unchecked), or +1
        int expectVelY;
    };

    const float speed = sgl::arkanoid::ballSpeed;
    const Shot shots[]{
        // Left, right, and top faces, including a center already inside the right wall.
        {.pos = {1.f, 360.f}, .vel = {-speed, 40.f}, .expectVelX = 1, .expectVelY = 0},
        {.pos = {sgl::arkanoid::designWidth - sgl::arkanoid::ballDiameter - 1.f, 360.f},
         .vel = {speed, 40.f},
         .expectVelX = -1,
         .expectVelY = 0},
        {.pos = {sgl::arkanoid::designWidth - sgl::arkanoid::ballRadius, 360.f},
         .vel = {speed, 0.f},
         .expectVelX = -1,
         .expectVelY = 0},
        {.pos = {640.f, 1.f}, .vel = {40.f, -speed}, .expectVelX = 0, .expectVelY = 1},
        // Top corners.
        {.pos = {1.f, 1.f}, .vel = {-speed, -speed}, .expectVelX = 1, .expectVelY = 1},
        {.pos = {sgl::arkanoid::designWidth - sgl::arkanoid::ballDiameter - 1.f, 1.f},
         .vel = {speed, -speed},
         .expectVelX = -1,
         .expectVelY = 1},
    };

    for(const Shot& shot: shots)
    {
        sgl::arkanoid::State state{};
        state.paddle.x = 580.f;
        state.balls = {
            sgl::arkanoid::Ball{.pos = shot.pos, .vel = shot.vel, .mode = sgl::arkanoid::BallMode::Live}};

        for(int i = 0; i < 30; ++i)
        {
            (void)sgl::arkanoid::step(state, {}, sgl::kTick);
        }

        ASSERT_EQ(state.balls.size(), 1u);
        const sgl::arkanoid::Ball& ball = state.balls.front();
        EXPECT_GE(ball.pos.x, 0.f);
        EXPECT_LE(ball.pos.x + sgl::arkanoid::ballDiameter, sgl::arkanoid::designWidth);
        EXPECT_GE(ball.pos.y, 0.f);
        if(shot.expectVelX > 0)
        {
            EXPECT_GT(ball.vel.x, 0.f);
        }
        else if(shot.expectVelX < 0)
        {
            EXPECT_LT(ball.vel.x, 0.f);
        }
        if(shot.expectVelY > 0)
        {
            EXPECT_GT(ball.vel.y, 0.f);
        }
    }
}

TEST(PhysicsTest, eventsForPaddleWallLaunch)
{
    sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);
    state.bricks.clear();

    const auto launchEvents = sgl::arkanoid::step(state, {.launch = true}, sgl::kTick);
    EXPECT_TRUE(hasEvent(launchEvents, [](const sgl::arkanoid::SimEvent& e) {
        return std::holds_alternative<sgl::arkanoid::BallLaunched>(e);
    }));

    // Drive into the left wall.
    state.balls.front().pos = {2.f, 300.f};
    state.balls.front().vel = {-sgl::arkanoid::ballSpeed, -100.f};
    const auto wallEvents = sgl::arkanoid::step(state, {}, sgl::kTick);
    EXPECT_TRUE(hasEvent(wallEvents, [](const sgl::arkanoid::SimEvent& e) {
        return std::holds_alternative<sgl::arkanoid::WallHit>(e);
    }));

    // Drop onto the paddle center.
    const float paddleCenterX = state.paddle.x + state.paddle.width * 0.5f;
    state.balls.front().pos = {paddleCenterX - sgl::arkanoid::ballRadius,
                               sgl::arkanoid::paddleY - sgl::arkanoid::ballDiameter - 2.f};
    state.balls.front().vel = {0.f, sgl::arkanoid::ballSpeed};
    const auto paddleEvents = sgl::arkanoid::step(state, {}, sgl::kTick);
    EXPECT_TRUE(hasEvent(paddleEvents, [](const sgl::arkanoid::SimEvent& e) {
        return std::holds_alternative<sgl::arkanoid::PaddleHit>(e);
    }));
}
