#include <algorithm>
#include <arkanoid/sim/Effects.hpp>
#include <arkanoid/sim/Physics.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <cmath>
#include <cstddef>
#include <numbers>
#include <optional>
#include <sgl/collision/Circle.hpp>
#include <sgl/collision/Collision.hpp>
#include <sgl/core/Overloaded.hpp>
#include <utility>
#include <variant>
#include <vector>

namespace sgl::arkanoid
{
namespace
{

constexpr float paddleMaxBounceAngle = 60.f * std::numbers::pi_v<float> / 180.f;
constexpr float minVerticalAngle = 15.f * std::numbers::pi_v<float> / 180.f;
constexpr float wallThickness = 64.f;
constexpr float seamEpsilon = 1e-4f;
constexpr float toiEpsilon = 1e-6f;
constexpr int maxToiIterations = 4;

enum class ObstacleKind
{
    Brick,
    Wall,
    Paddle
};

struct ObstacleHit
{
    float time{};
    sgl::Vec2f normal{};
    ObstacleKind kind{};
    std::size_t brickIndex{};
};

struct WideCapsule
{
};

struct SlowCapsule
{
};

struct MultiBallCapsule
{
};

struct ExtraLifeCapsule
{
};

using PowerUp = std::variant<WideCapsule, SlowCapsule, MultiBallCapsule, ExtraLifeCapsule>;

[[nodiscard]] PowerUp powerUpFromKind(PowerUpKind kind)
{
    switch(kind)
    {
        case PowerUpKind::Wide:
            return WideCapsule{};
        case PowerUpKind::Slow:
            return SlowCapsule{};
        case PowerUpKind::MultiBall:
            return MultiBallCapsule{};
        case PowerUpKind::ExtraLife:
            return ExtraLifeCapsule{};
    }
    std::unreachable();
}

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

void syncPaddleWidth(State& state)
{
    state.paddle.width = paddleWidth(state.effects);
    clampPaddle(state.paddle);
}

void enforceMinVertical(Ball& ball, float speed);

void setBallSpeed(Ball& ball, float speed)
{
    if(const auto dir = sgl::normalized(ball.vel))
    {
        ball.vel = *dir * speed;
    }
    else
    {
        ball.vel = {0.f, -speed};
    }
    enforceMinVertical(ball, speed);
}

[[nodiscard]] float targetBallSpeed(const State& state)
{
    return ballSpeed * ballSpeedMultiplier(state);
}

void syncLiveBallSpeeds(State& state)
{
    const float speed = targetBallSpeed(state);
    for(auto& ball: state.balls)
    {
        if(ball.mode != BallMode::Live || offBottom(ball))
        {
            continue;
        }
        setBallSpeed(ball, speed);
    }
}

void refreshOrAddTimed(Effects& effects, TimedEffect effect)
{
    for(TimedEffect& existing: effects.active)
    {
        if(existing.index() == effect.index())
        {
            existing = effect;
            return;
        }
    }
    effects.active.push_back(effect);
}

void tickTimedEffects(State& state, sgl::Seconds dt)
{
    for(TimedEffect& effect: state.effects.active)
    {
        std::visit([&](auto& timed) { timed.remaining -= dt; }, effect);
    }

    const bool hadWide = std::any_of(state.effects.active.begin(),
                                     state.effects.active.end(),
                                     [](const TimedEffect& e) { return std::holds_alternative<Wide>(e); });
    const bool hadSlow = std::any_of(state.effects.active.begin(),
                                     state.effects.active.end(),
                                     [](const TimedEffect& e) { return std::holds_alternative<Slow>(e); });

    std::erase_if(state.effects.active, [](const TimedEffect& effect) {
        return std::visit([](const auto& timed) { return timed.remaining.count() <= 0.f; }, effect);
    });

    const bool hasWide = std::any_of(state.effects.active.begin(),
                                     state.effects.active.end(),
                                     [](const TimedEffect& e) { return std::holds_alternative<Wide>(e); });
    const bool hasSlow = std::any_of(state.effects.active.begin(),
                                     state.effects.active.end(),
                                     [](const TimedEffect& e) { return std::holds_alternative<Slow>(e); });

    if(hadWide && !hasWide)
    {
        syncPaddleWidth(state);
    }
    if(hadSlow && !hasSlow)
    {
        syncLiveBallSpeeds(state);
    }
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
        const float speed = targetBallSpeed(state);
        if(const auto dir = sgl::normalized(launchVelocity))
        {
            const sgl::Vec2f base = *dir * speed;
            leftVel = {base.x - multiballSpread, base.y};
            rightVel = {base.x + multiballSpread, base.y};
        }
        else
        {
            leftVel = {-multiballSpread, -speed};
            rightVel = {multiballSpread, -speed};
        }
    }
    else
    {
        leftVel = {first.vel.x - multiballSpread, first.vel.y};
        rightVel = {first.vel.x + multiballSpread, first.vel.y};
    }

    const float speed = targetBallSpeed(state);
    Ball left{.pos = pos, .vel = leftVel, .mode = BallMode::Live};
    Ball right{.pos = pos, .vel = rightVel, .mode = BallMode::Live};
    setBallSpeed(left, speed);
    setBallSpeed(right, speed);
    state.balls.push_back(left);
    state.balls.push_back(right);
}

void enforceMinVertical(Ball& ball, float speed)
{
    const float minVy = speed * std::sin(minVerticalAngle);
    if(std::abs(ball.vel.y) >= minVy)
    {
        return;
    }

    const float signY = ball.vel.y < 0.f ? -1.f : 1.f;
    const float signX = ball.vel.x < 0.f ? -1.f : 1.f;
    const float vx = std::sqrt(std::max(0.f, speed * speed - minVy * minVy));
    ball.vel = {signX * vx, signY * minVy};
}

void applyPaddleBounce(Ball& ball, const Paddle& paddle, float speed)
{
    const float paddleCenterX = paddle.x + paddle.width * 0.5f;
    float hitOffset = (ballCenter(ball).x - paddleCenterX) / (paddle.width * 0.5f);
    hitOffset = std::clamp(hitOffset, -1.f, 1.f);
    const float angle = hitOffset * paddleMaxBounceAngle;
    ball.vel = {speed * std::sin(angle), -speed * std::cos(angle)};
    enforceMinVertical(ball, speed);
}

void depenetrateBallFromPaddle(Ball& ball, const Paddle& paddle)
{
    if(ball.mode != BallMode::Live || !ballOverlapsPaddle(ball, paddle))
    {
        return;
    }

    constexpr float separation = 0.01f;
    const float paddleCenterY = paddleY + paddleHeight * 0.5f;
    if(ballCenter(ball).y <= paddleCenterY)
    {
        ball.pos.y = paddleY - ballDiameter - separation;
        return;
    }

    if(paddle.speed >= 0.f)
    {
        ball.pos.x = paddle.x + paddle.width + separation;
    }
    else
    {
        ball.pos.x = paddle.x - ballDiameter - separation;
    }
    ball.pos.x = std::clamp(ball.pos.x, 0.f, designWidth - ballDiameter);
}

[[nodiscard]] sgl::Aabb leftWallBox()
{
    return {{-wallThickness, -wallThickness}, {wallThickness, designHeight + 2.f * wallThickness}};
}

[[nodiscard]] sgl::Aabb rightWallBox()
{
    return {{designWidth, -wallThickness}, {wallThickness, designHeight + 2.f * wallThickness}};
}

[[nodiscard]] sgl::Aabb topWallBox()
{
    return {{-wallThickness, -wallThickness}, {designWidth + 2.f * wallThickness, wallThickness}};
}

[[nodiscard]] sgl::Aabb paddleBox(const Paddle& paddle)
{
    return {{paddle.x, paddleY}, {paddle.width, paddleHeight}};
}

void bounceOutOfWalls(Ball& ball, sgl::Vec2f hitNormal, float speed)
{
    constexpr float separation = 0.01f;
    bool overlapped = false;
    const sgl::Aabb walls[]{leftWallBox(), rightWallBox(), topWallBox()};
    for(const sgl::Aabb& wall: walls)
    {
        const sgl::Circle circle{.center = ballCenter(ball), .radius = ballRadius};
        const auto contact = sgl::intersect(circle, wall);
        if(!contact || contact->depth <= 0.f)
        {
            continue;
        }
        overlapped = true;
        ball.vel = sgl::reflect(ball.vel, contact->normal);
        ball.pos = ball.pos + contact->normal * (contact->depth + separation);
    }
    if(!overlapped)
    {
        ball.vel = sgl::reflect(ball.vel, hitNormal);
        ball.pos = ball.pos + hitNormal * separation;
    }
    setBallSpeed(ball, speed);
}

void considerHit(
    std::optional<ObstacleHit>& best, float time, sgl::Vec2f normal, ObstacleKind kind, std::size_t brickIndex = 0)
{
    if(time < 0.f || time > 1.f)
    {
        return;
    }
    if(!best.has_value() || time < best->time)
    {
        best = ObstacleHit{.time = time, .normal = normal, .kind = kind, .brickIndex = brickIndex};
    }
}

void resolveBrickHits(State& state,
                      Ball& ball,
                      float tBest,
                      sgl::Vec2f bestNormal,
                      const sgl::Circle& circle,
                      sgl::Vec2f delta,
                      float speed,
                      std::vector<SimEvent>& events)
{
    sgl::Vec2f normalSum{};
    bool any = false;
    for(std::size_t i = 0; i < state.bricks.size(); ++i)
    {
        if(!state.bricks[i].alive)
        {
            continue;
        }
        const auto hit = sgl::sweep(circle, delta, state.bricks[i].box);
        if(!hit || std::abs(hit->time - tBest) >= seamEpsilon)
        {
            continue;
        }

        Brick& brick = state.bricks[i];
        brick.alive = false;
        state.score += scorePerBrick;
        events.push_back(BrickDestroyed{.index = i});
        if(brick.drop.has_value())
        {
            state.capsules.push_back(Capsule{.pos = brick.box.pos, .kind = *brick.drop});
        }
        normalSum = normalSum + hit->normal;
        any = true;
    }

    if(!any)
    {
        return;
    }

    sgl::Vec2f normal = bestNormal;
    if(const auto sumDir = sgl::normalized(normalSum))
    {
        normal = *sumDir;
    }
    ball.vel = sgl::reflect(ball.vel, normal);
    setBallSpeed(ball, speed);
}

void integrateLiveBall(State& state, Ball& ball, float dt, std::vector<SimEvent>& events)
{
    float remaining = 1.f;
    for(int iter = 0; iter < maxToiIterations && remaining > toiEpsilon; ++iter)
    {
        const float speed = targetBallSpeed(state);
        const sgl::Vec2f delta = ball.vel * (dt * remaining);
        const sgl::Circle circle{.center = ballCenter(ball), .radius = ballRadius};

        std::optional<ObstacleHit> best{};
        for(std::size_t i = 0; i < state.bricks.size(); ++i)
        {
            if(!state.bricks[i].alive)
            {
                continue;
            }
            if(const auto hit = sgl::sweep(circle, delta, state.bricks[i].box))
            {
                considerHit(best, hit->time, hit->normal, ObstacleKind::Brick, i);
            }
        }

        if(const auto hit = sgl::sweep(circle, delta, leftWallBox()))
        {
            considerHit(best, hit->time, hit->normal, ObstacleKind::Wall);
        }
        if(const auto hit = sgl::sweep(circle, delta, rightWallBox()))
        {
            considerHit(best, hit->time, hit->normal, ObstacleKind::Wall);
        }
        if(const auto hit = sgl::sweep(circle, delta, topWallBox()))
        {
            considerHit(best, hit->time, hit->normal, ObstacleKind::Wall);
        }
        if(const auto hit = sgl::sweep(circle, delta, paddleBox(state.paddle)))
        {
            considerHit(best, hit->time, hit->normal, ObstacleKind::Paddle);
        }

        if(!best.has_value())
        {
            ball.pos = ball.pos + delta;
            break;
        }

        ball.pos = ball.pos + delta * best->time;
        remaining *= (1.f - best->time);

        switch(best->kind)
        {
            case ObstacleKind::Brick:
                resolveBrickHits(state, ball, best->time, best->normal, circle, delta, speed, events);
                break;
            case ObstacleKind::Wall:
                bounceOutOfWalls(ball, best->normal, speed);
                events.push_back(WallHit{});
                break;
            case ObstacleKind::Paddle:
                applyPaddleBounce(ball, state.paddle, speed);
                events.push_back(PaddleHit{});
                break;
        }

        if(best->time <= toiEpsilon && remaining >= 1.f - toiEpsilon)
        {
            // Stuck in contact with zero progress — stop to avoid infinite TOI loops.
            break;
        }
    }
}

} // namespace

float ballSpeedMultiplier(const State& state)
{
    return ballSpeedMultiplier(state.effects);
}

void applyPowerUp(State& state, PowerUpKind kind)
{
    std::visit(sgl::Overloaded{
                   [&](WideCapsule) {
                       refreshOrAddTimed(state.effects, Wide{.remaining = sgl::Seconds{effectDuration}});
                       syncPaddleWidth(state);
                   },
                   [&](SlowCapsule) {
                       refreshOrAddTimed(state.effects, Slow{.remaining = sgl::Seconds{effectDuration}});
                       syncLiveBallSpeeds(state);
                   },
                   [&](ExtraLifeCapsule) { ++state.lives; },
                   [&](MultiBallCapsule) { applyMultiBall(state); },
               },
               powerUpFromKind(kind));
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
        else
        {
            depenetrateBallFromPaddle(ball, state.paddle);
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
            if(const auto dir = sgl::normalized(launchVelocity))
            {
                ball.vel = *dir * targetBallSpeed(state);
            }
            else
            {
                ball.vel = {0.f, -targetBallSpeed(state)};
            }
            enforceMinVertical(ball, targetBallSpeed(state));
            events.push_back(BallLaunched{});
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

    tickTimedEffects(state, dt);

    if(!state.balls.empty())
    {
        const std::size_t fallen =
            static_cast<std::size_t>(std::count_if(state.balls.begin(), state.balls.end(), offBottom));
        if(fallen == state.balls.size())
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
        else if(fallen > 0)
        {
            for(std::size_t i = 0; i < fallen; ++i)
            {
                events.push_back(BallLost{});
            }
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
