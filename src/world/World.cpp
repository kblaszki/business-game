#include "World.hpp"

#include <Game.hpp>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace
{
constexpr float launchX{160.f};
constexpr float launchY{-360.f};

bool overlaps(sf::Vector2f center, float radius, sf::Vector2f pos, sf::Vector2f size)
{
    const float nearestX = std::clamp(center.x, pos.x, pos.x + size.x);
    const float nearestY = std::clamp(center.y, pos.y, pos.y + size.y);
    const float dx = center.x - nearestX;
    const float dy = center.y - nearestY;
    return dx * dx + dy * dy <= radius * radius;
}

sf::Vector2f launchVelocity()
{
    return {launchX, launchY};
}
} // namespace

World::World()
    : bat{{(static_cast<float>(Game::DESIGN_SIZE.x) - 120.f) * 0.5f, 680.f}, art.paddle}
    , sphere{{0.f, 0.f}, art.ball}
{
    stickBallToPaddle();
}

void World::setPaddleSpeed(float pxPerSec)
{
    bat.setSpeed(pxPerSec);
}

void World::launch()
{
    if(!sphere.stuck() || won() || lost())
    {
        return;
    }

    sphere.setStuck(false);
    sphere.setVelocity(launchVelocity());
}

void World::placeBall(sf::Vector2f position, sf::Vector2f velocity, bool stuck)
{
    sphere.setPosition(position);
    sphere.setVelocity(velocity);
    sphere.setStuck(stuck);
}

void World::addBrick(sf::Vector2f position, sf::Color tint)
{
    bricks.emplace_back(position, tint, art.brick);
}

void World::fixedUpdate(sf::Time tick)
{
    ++ticks;
    if(won() || lost())
    {
        return;
    }

    bat.fixedUpdate(tick);
    if(sphere.stuck())
    {
        stickBallToPaddle();
        return;
    }

    sphere.fixedUpdate(tick);
    bounceWalls();
    bouncePaddle();
    bounceBricks();
    missCheck();
}

void World::draw(DrawerI& drawer) const
{
    drawer.draw(art.background);
    for(const auto& brick: bricks)
    {
        brick.draw(drawer);
    }
    bat.draw(drawer);
    sphere.draw(drawer);
}

std::uint32_t World::tickCount() const
{
    return ticks;
}

std::uint32_t World::score() const
{
    return points;
}

std::uint32_t World::lives() const
{
    return remaining;
}

bool World::won() const
{
    if(bricks.empty())
    {
        return false;
    }

    return std::none_of(bricks.begin(), bricks.end(), [](const Brick& brick) { return brick.alive(); });
}

bool World::lost() const
{
    return remaining == 0;
}

const Paddle& World::paddle() const
{
    return bat;
}

const Ball& World::ball() const
{
    return sphere;
}

std::size_t World::brickCount() const
{
    return bricks.size();
}

const Brick& World::brickAt(std::size_t index) const
{
    if(index >= bricks.size())
    {
        throw std::out_of_range{"World::brickAt"};
    }

    return bricks[index];
}

void World::stickBallToPaddle()
{
    const auto paddlePos = bat.position();
    const auto paddleSize = bat.size();
    const auto ballSize = sphere.size();
    sphere.setPosition({paddlePos.x + (paddleSize.x - ballSize.x) * 0.5f, paddlePos.y - ballSize.y});
    sphere.setVelocity({});
    sphere.setStuck(true);
}

void World::bounceWalls()
{
    auto pos = sphere.position();
    auto vel = sphere.velocity();
    const auto size = sphere.size();
    const float width = static_cast<float>(Game::DESIGN_SIZE.x);

    if(pos.x <= 0.f)
    {
        pos.x = 0.f;
        vel.x = std::abs(vel.x);
    }
    if(pos.x + size.x >= width)
    {
        pos.x = width - size.x;
        vel.x = -std::abs(vel.x);
    }
    if(pos.y <= 0.f)
    {
        pos.y = 0.f;
        vel.y = std::abs(vel.y);
    }

    sphere.setPosition(pos);
    sphere.setVelocity(vel);
}

void World::bouncePaddle()
{
    if(sphere.velocity().y <= 0.f)
    {
        return;
    }

    if(!overlaps(sphere.center(), sphere.radius(), bat.position(), bat.size()))
    {
        return;
    }

    auto vel = sphere.velocity();
    vel.y = -std::abs(vel.y);
    const float hit = (sphere.center().x - (bat.position().x + bat.size().x * 0.5f)) / (bat.size().x * 0.5f);
    vel.x += hit * 120.f;
    sphere.setVelocity(vel);
    sphere.setPosition({sphere.position().x, bat.position().y - sphere.size().y});
}

void World::bounceBricks()
{
    for(auto& brick: bricks)
    {
        if(!brick.alive())
        {
            continue;
        }

        if(!overlaps(sphere.center(), sphere.radius(), brick.position(), brick.size()))
        {
            continue;
        }

        brick.kill();
        points += 10;

        const auto brickPos = brick.position();
        const auto brickSize = brick.size();
        const auto center = sphere.center();
        const float overlapX =
            sphere.radius()
            - std::min(std::abs(center.x - brickPos.x), std::abs(center.x - (brickPos.x + brickSize.x)));
        const float overlapY =
            sphere.radius()
            - std::min(std::abs(center.y - brickPos.y), std::abs(center.y - (brickPos.y + brickSize.y)));

        auto vel = sphere.velocity();
        if(overlapX < overlapY)
        {
            vel.x = -vel.x;
        }
        else
        {
            vel.y = -vel.y;
        }
        sphere.setVelocity(vel);
        return;
    }
}

void World::missCheck()
{
    const float height = static_cast<float>(Game::DESIGN_SIZE.y);
    if(sphere.position().y < height)
    {
        return;
    }

    if(remaining > 0)
    {
        --remaining;
    }

    if(remaining > 0)
    {
        stickBallToPaddle();
    }
}
