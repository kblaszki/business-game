#include <algorithm>
#include <arkanoid/sim/Levels.hpp>
#include <arkanoid/sim/Physics.hpp>
#include <arkanoid/sim/State.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <cmath>
#include <gtest/gtest.h>
#include <numbers>
#include <sgl/collision/Circle.hpp>
#include <sgl/collision/Collision.hpp>
#include <sgl/core/Random.hpp>
#include <sgl/core/Time.hpp>
#include <sgl/core/Vec2.hpp>

namespace
{

[[nodiscard]] bool ballInsideArenaOrExitingBottom(const sgl::arkanoid::Ball& ball)
{
    if(ball.pos.y >= sgl::arkanoid::designHeight)
    {
        return true;
    }
    return ball.pos.x >= -0.01f && ball.pos.x + sgl::arkanoid::ballDiameter <= sgl::arkanoid::designWidth + 0.01f
           && ball.pos.y >= -0.01f;
}

[[nodiscard]] bool overlapsAliveBrick(const sgl::arkanoid::Ball& ball, const sgl::arkanoid::State& state)
{
    const sgl::Circle circle{.center = {ball.pos.x + sgl::arkanoid::ballRadius, ball.pos.y + sgl::arkanoid::ballRadius},
                             .radius = sgl::arkanoid::ballRadius};
    for(const auto& brick: state.bricks)
    {
        if(!brick.alive)
        {
            continue;
        }
        if(const auto contact = sgl::intersect(circle, brick.box))
        {
            if(contact->depth > 0.01f)
            {
                return true;
            }
        }
    }
    return false;
}

} // namespace

TEST(SimPropertyTest, invariantsHoldOverLongRuns)
{
    const float minSin = std::sin(15.f * std::numbers::pi_v<float> / 180.f);

    for(std::uint32_t seed = 0; seed < 20; ++seed)
    {
        sgl::Pcg32 rng{seed + 1u};
        sgl::arkanoid::State state = sgl::arkanoid::makeState(sgl::arkanoid::StageId::Stage1);
        float paddleAxis = 0.f;

        for(int tick = 0; tick < 10000; ++tick)
        {
            if(tick % 30 == 0)
            {
                paddleAxis = rng.uniformFloat(-1.f, 1.f);
            }

            sgl::arkanoid::SimInput input{.paddleAxis = paddleAxis};
            if(!state.balls.empty()
               && std::any_of(state.balls.begin(), state.balls.end(), [](const sgl::arkanoid::Ball& b) {
                      return b.mode == sgl::arkanoid::BallMode::Stuck;
                  }))
            {
                input.launch = true;
            }

            (void)sgl::arkanoid::step(state, input, sgl::kTick);

            if(state.over || state.cleared)
            {
                break;
            }

            for(const auto& ball: state.balls)
            {
                if(ball.mode != sgl::arkanoid::BallMode::Live)
                {
                    continue;
                }

                EXPECT_TRUE(ballInsideArenaOrExitingBottom(ball)) << "seed=" << seed << " tick=" << tick;
                EXPECT_FALSE(overlapsAliveBrick(ball, state)) << "seed=" << seed << " tick=" << tick;

                const float speed = std::sqrt(sgl::lengthSq(ball.vel));
                const float expected = sgl::arkanoid::ballSpeed * sgl::arkanoid::ballSpeedMultiplier(state);
                EXPECT_NEAR(speed, expected, expected * 1e-3f) << "seed=" << seed << " tick=" << tick;
                ASSERT_GT(speed, 0.f);
                EXPECT_GE(std::abs(ball.vel.y) / speed, minSin - 1e-4f) << "seed=" << seed << " tick=" << tick;
            }
        }
    }
}
