#include <algorithm>
#include <arkanoid/sim/Physics.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <cmath>
#include <cstddef>
#include <optional>
#include <sgl/collision/Circle.hpp>
#include <sgl/collision/Collision.hpp>

namespace sgl::arkanoid
{
namespace
{

[[nodiscard]] sgl::Vec2f ballCenter(const Ball& ball)
{
    return {ball.pos.x + ballRadius, ball.pos.y + ballRadius};
}

[[nodiscard]] bool offBottom(const Ball& ball)
{
    return ball.pos.y >= designHeight;
}

[[nodiscard]] bool aabbOverlap(sgl::Vec2f aPos, sgl::Vec2f aSize, sgl::Vec2f bPos, sgl::Vec2f bSize)
{
    return aPos.x < bPos.x + bSize.x && aPos.x + aSize.x > bPos.x && aPos.y < bPos.y + bSize.y
           && aPos.y + aSize.y > bPos.y;
}

[[nodiscard]] bool ballOverlapsPaddle(const Ball& ball, const Paddle& paddle)
{
    const sgl::Vec2f center = ballCenter(ball);
    const float nearestX = std::clamp(center.x, paddle.x, paddle.x + paddle.width);
    const float nearestY = std::clamp(center.y, paddleY, paddleY + paddleHeight);
    const float dx = center.x - nearestX;
    const float dy = center.y - nearestY;
    return dx * dx + dy * dy <= ballRadius * ballRadius;
}

void stickBallToPaddle(Ball& ball, const Paddle& paddle)
{
    ball.pos = {paddle.x + (paddle.width - ballDiameter) * 0.5f, paddleY - ballDiameter};
    ball.vel = {};
    ball.mode = BallMode::Stuck;
}

void clampPaddle(Paddle& paddle)
{
    const float maxX = designWidth - paddle.width;
    paddle.x = std::clamp(paddle.x, 0.f, maxX);
}

void expireTimedEffect(State& state)
{
    if(!state.effects.timed.has_value())
    {
        state.effects.remaining = sgl::Seconds{0.f};
        return;
    }

    if(*state.effects.timed == PowerUpKind::Wide)
    {
        state.paddle.width = paddleWidth;
        clampPaddle(state.paddle);
    }
    else if(*state.effects.timed == PowerUpKind::Slow && state.effects.slowActive)
    {
        for(auto& ball: state.balls)
        {
            if(!offBottom(ball))
            {
                ball.vel = ball.vel * (1.f / slowFactor);
            }
        }
        state.effects.slowActive = false;
    }

    state.effects.timed.reset();
    state.effects.remaining = sgl::Seconds{0.f};
}

void applyMultiBall(State& state)
{
    if(state.balls.empty())
    {
        return;
    }

    const Ball& first = state.balls.front();
    const sgl::Vec2f pos = first.pos;
    sgl::Vec2f leftVel{};
    sgl::Vec2f rightVel{};
    if(first.mode == BallMode::Stuck)
    {
        leftVel = {launchVelocity.x - multiballSpread, launchVelocity.y};
        rightVel = {launchVelocity.x + multiballSpread, launchVelocity.y};
        if(state.effects.slowActive)
        {
            leftVel = leftVel * slowFactor;
            rightVel = rightVel * slowFactor;
        }
    }
    else
    {
        leftVel = {first.vel.x - multiballSpread, first.vel.y};
        rightVel = {first.vel.x + multiballSpread, first.vel.y};
    }

    state.balls.push_back(Ball{.pos = pos, .vel = leftVel, .mode = BallMode::Live});
    state.balls.push_back(Ball{.pos = pos, .vel = rightVel, .mode = BallMode::Live});
}

void bounceWalls(Ball& ball)
{
    if(ball.pos.x <= 0.f)
    {
        ball.pos.x = 0.f;
        ball.vel.x = std::abs(ball.vel.x);
    }
    if(ball.pos.x + ballDiameter >= designWidth)
    {
        ball.pos.x = designWidth - ballDiameter;
        ball.vel.x = -std::abs(ball.vel.x);
    }
    if(ball.pos.y <= 0.f)
    {
        ball.pos.y = 0.f;
        ball.vel.y = std::abs(ball.vel.y);
    }
}

void bouncePaddle(Ball& ball, const Paddle& paddle)
{
    if(ball.vel.y <= 0.f)
    {
        return;
    }
    if(!ballOverlapsPaddle(ball, paddle))
    {
        return;
    }

    ball.vel.y = -std::abs(ball.vel.y);
    const float paddleCenterX = paddle.x + paddle.width * 0.5f;
    const float hitOffset = (ballCenter(ball).x - paddleCenterX) / (paddle.width * 0.5f);
    ball.vel.x += hitOffset * 120.f;
    ball.pos.y = paddleY - ballDiameter;
}

void resolveBrickHit(State& state, Ball& ball, std::size_t brickIndex, sgl::Vec2f normal, std::vector<SimEvent>& events)
{
    Brick& brick = state.bricks[brickIndex];
    if(!brick.alive)
    {
        return;
    }

    brick.alive = false;
    state.score += scorePerBrick;
    events.push_back(BrickDestroyed{.index = brickIndex});

    if(brick.drop.has_value())
    {
        state.capsules.push_back(Capsule{.pos = brick.box.pos, .kind = *brick.drop});
    }

    ball.vel = sgl::reflect(ball.vel, normal);
}

void integrateLiveBall(State& state, Ball& ball, float dt, std::vector<SimEvent>& events)
{
    const sgl::Vec2f delta = ball.vel * dt;
    const sgl::Circle circle{.center = ballCenter(ball), .radius = ballRadius};

    std::optional<sgl::Hit> best{};
    std::size_t bestIndex = 0;
    for(std::size_t i = 0; i < state.bricks.size(); ++i)
    {
        if(!state.bricks[i].alive)
        {
            continue;
        }

        if(const auto hit = sgl::sweep(circle, delta, state.bricks[i].box))
        {
            if(!best.has_value() || hit->time < best->time)
            {
                best = hit;
                bestIndex = i;
            }
        }
    }

    if(best.has_value())
    {
        ball.pos = ball.pos + delta * best->time;
        resolveBrickHit(state, ball, bestIndex, best->normal, events);
    }
    else
    {
        ball.pos = ball.pos + delta;
    }

    bounceWalls(ball);
    bouncePaddle(ball, state.paddle);
}

} // namespace

void applyPowerUp(State& state, PowerUpKind kind)
{
    switch(kind)
    {
        case PowerUpKind::Wide:
            expireTimedEffect(state);
            state.paddle.width = paddleWideWidth;
            clampPaddle(state.paddle);
            state.effects.timed = PowerUpKind::Wide;
            state.effects.remaining = sgl::Seconds{effectDuration};
            break;
        case PowerUpKind::Slow:
            expireTimedEffect(state);
            for(auto& ball: state.balls)
            {
                ball.vel = ball.vel * slowFactor;
            }
            state.effects.slowActive = true;
            state.effects.timed = PowerUpKind::Slow;
            state.effects.remaining = sgl::Seconds{effectDuration};
            break;
        case PowerUpKind::ExtraLife:
            ++state.lives;
            break;
        case PowerUpKind::MultiBall:
            applyMultiBall(state);
            break;
    }
}

std::vector<SimEvent> step(State& state, const SimInput& input, sgl::Seconds dt)
{
    std::vector<SimEvent> events;
    if(state.cleared || state.over)
    {
        return events;
    }

    const float dtSec = dt.count();

    state.paddle.speed = input.paddleAxis * paddleSpeed;
    state.paddle.x += state.paddle.speed * dtSec;
    clampPaddle(state.paddle);

    for(auto& ball: state.balls)
    {
        if(ball.mode == BallMode::Stuck)
        {
            stickBallToPaddle(ball, state.paddle);
        }
    }

    if(input.launch)
    {
        for(auto& ball: state.balls)
        {
            if(ball.mode != BallMode::Stuck)
            {
                continue;
            }

            ball.mode = BallMode::Live;
            sgl::Vec2f vel = launchVelocity;
            if(state.effects.slowActive)
            {
                vel = vel * slowFactor;
            }
            ball.vel = vel;
            break;
        }
    }

    for(auto& ball: state.balls)
    {
        if(ball.mode != BallMode::Live)
        {
            continue;
        }
        if(offBottom(ball))
        {
            continue;
        }
        integrateLiveBall(state, ball, dtSec, events);
    }

    for(auto& capsule: state.capsules)
    {
        capsule.pos.y += capsuleFallSpeed * dtSec;
    }

    std::erase_if(state.capsules, [](const Capsule& capsule) { return capsule.pos.y >= designHeight; });

    {
        std::size_t i = 0;
        while(i < state.capsules.size())
        {
            const sgl::Vec2f paddlePos{state.paddle.x, paddleY};
            const sgl::Vec2f paddleSize{state.paddle.width, paddleHeight};
            if(aabbOverlap(state.capsules[i].pos, {capsuleWidth, capsuleHeight}, paddlePos, paddleSize))
            {
                const PowerUpKind kind = state.capsules[i].kind;
                state.capsules.erase(state.capsules.begin() + static_cast<std::ptrdiff_t>(i));
                applyPowerUp(state, kind);
                events.push_back(PowerUpCaught{.kind = kind});
            }
            else
            {
                ++i;
            }
        }
    }

    if(state.effects.remaining.count() > 0.f)
    {
        state.effects.remaining -= dt;
        if(state.effects.remaining.count() <= 0.f)
        {
            expireTimedEffect(state);
        }
    }

    if(!state.balls.empty())
    {
        const bool allFallen = std::all_of(state.balls.begin(), state.balls.end(), offBottom);
        if(allFallen)
        {
            events.push_back(LifeLost{});
            if(state.lives > 0)
            {
                --state.lives;
            }

            if(state.lives > 0)
            {
                state.balls.clear();
                Ball ball{};
                stickBallToPaddle(ball, state.paddle);
                state.balls.push_back(ball);
            }
            else
            {
                state.over = true;
                events.push_back(GameOver{});
            }
        }
        else
        {
            std::erase_if(state.balls, offBottom);
        }
    }

    if(!state.bricks.empty()
       && std::none_of(state.bricks.begin(), state.bricks.end(), [](const Brick& brick) { return brick.alive; }))
    {
        state.cleared = true;
        events.push_back(StageCleared{.stage = state.stage});
    }

    return events;
}

} // namespace sgl::arkanoid
