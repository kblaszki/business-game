#include "World.hpp"

#include <Game.hpp>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace
{
constexpr float launchX{160.f};
constexpr float launchY{-360.f};
constexpr float defaultPaddleWidth{120.f};
constexpr float widePaddleWidth{200.f};
constexpr float slowFactor{0.6f};
constexpr float multiSpread{120.f};
constexpr std::uint32_t timedTicks{480};

bool overlaps(sf::Vector2f center, float radius, sf::Vector2f pos, sf::Vector2f size)
{
    const float nearestX = std::clamp(center.x, pos.x, pos.x + size.x);
    const float nearestY = std::clamp(center.y, pos.y, pos.y + size.y);
    const float dx = center.x - nearestX;
    const float dy = center.y - nearestY;
    return dx * dx + dy * dy <= radius * radius;
}

bool aabbOverlap(sf::Vector2f aPos, sf::Vector2f aSize, sf::Vector2f bPos, sf::Vector2f bSize)
{
    return aPos.x < bPos.x + bSize.x && aPos.x + aSize.x > bPos.x && aPos.y < bPos.y + bSize.y
           && aPos.y + aSize.y > bPos.y;
}

sf::Vector2f launchVelocity()
{
    return {launchX, launchY};
}

bool offScreen(const Ball& sphere)
{
    return sphere.position().y >= static_cast<float>(Game::DESIGN_SIZE.y);
}
} // namespace

World::World()
    : bat{{(static_cast<float>(Game::DESIGN_SIZE.x) - defaultPaddleWidth) * 0.5f, 680.f}, art.paddle}
{
    spheres.emplace_back(sf::Vector2f{0.f, 0.f}, art.ball);
    stickBallToPaddle();
}

void World::setPaddleSpeed(float pxPerSec)
{
    bat.setSpeed(pxPerSec);
}

void World::launch()
{
    if(won() || lost())
    {
        return;
    }

    for(auto& sphere: spheres)
    {
        if(!sphere.stuck())
        {
            continue;
        }

        sphere.setStuck(false);
        auto vel = launchVelocity();
        if(slowActive)
        {
            vel *= slowFactor;
        }
        sphere.setVelocity(vel);
        return;
    }
}

void World::placeBall(sf::Vector2f position, sf::Vector2f velocity, bool stuck)
{
    if(spheres.empty())
    {
        spheres.emplace_back(position, art.ball);
    }

    spheres.front().setPosition(position);
    spheres.front().setVelocity(velocity);
    spheres.front().setStuck(stuck);
}

void World::addBrick(sf::Vector2f position, sf::Color tint)
{
    bricks.emplace_back(position, tint, art.brick);
}

void World::killBrick(std::size_t index)
{
    if(index >= bricks.size())
    {
        throw std::out_of_range{"World::killBrick"};
    }

    if(!bricks[index].alive())
    {
        return;
    }

    bricks[index].kill();
    points += 10;
    spawnDonorCapsule(index);
}

void World::setScore(std::uint32_t value)
{
    points = value;
}

void World::setLives(std::uint32_t value)
{
    remaining = value;
}

void World::applyPowerUp(PowerUpKind kind)
{
    switch(kind)
    {
        case PowerUpKind::Wide:
            expireTimedEffect();
            bat.setDisplayWidth(widePaddleWidth);
            timedEffect = PowerUpKind::Wide;
            timedRemaining = timedTicks;
            break;
        case PowerUpKind::Slow:
            expireTimedEffect();
            for(auto& sphere: spheres)
            {
                sphere.setVelocity(sphere.velocity() * slowFactor);
            }
            slowActive = true;
            timedEffect = PowerUpKind::Slow;
            timedRemaining = timedTicks;
            break;
        case PowerUpKind::ExtraLife:
            ++remaining;
            break;
        case PowerUpKind::MultiBall:
            applyMultiBall();
            break;
    }
}

void World::fixedUpdate(sf::Time tick)
{
    ++ticks;
    if(won() || lost())
    {
        updatePowerUps(tick);
        return;
    }

    tickTimedEffect();
    bat.fixedUpdate(tick);

    for(auto& sphere: spheres)
    {
        if(sphere.stuck())
        {
            stickBall(sphere);
            continue;
        }

        if(offScreen(sphere))
        {
            continue;
        }

        sphere.fixedUpdate(tick);
        bounceWalls(sphere);
        bouncePaddle(sphere);
        bounceBricks(sphere);
    }

    updatePowerUps(tick);
    missCheck();
}

void World::draw(DrawerI& drawer) const
{
    drawer.draw(art.background);
    for(const auto& brick: bricks)
    {
        brick.draw(drawer);
    }
    for(const auto& capsule: capsules)
    {
        capsule.draw(drawer);
    }
    bat.draw(drawer);
    for(const auto& sphere: spheres)
    {
        if(!offScreen(sphere))
        {
            sphere.draw(drawer);
        }
    }
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
    return spheres.front();
}

std::size_t World::ballCount() const
{
    return spheres.size();
}

const Ball& World::ballAt(std::size_t index) const
{
    if(index >= spheres.size())
    {
        throw std::out_of_range{"World::ballAt"};
    }

    return spheres[index];
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

std::size_t World::powerUpCount() const
{
    return capsules.size();
}

const PowerUp& World::powerUpAt(std::size_t index) const
{
    if(index >= capsules.size())
    {
        throw std::out_of_range{"World::powerUpAt"};
    }

    return capsules[index];
}

std::string World::activePowerUpName() const
{
    if(!timedEffect.has_value() || timedRemaining == 0)
    {
        return {};
    }

    switch(*timedEffect)
    {
        case PowerUpKind::Wide:
            return "Wide";
        case PowerUpKind::Slow:
            return "Slow";
        case PowerUpKind::MultiBall:
        case PowerUpKind::ExtraLife:
            break;
    }

    return {};
}

void World::stickBall(Ball& sphere)
{
    const auto paddlePos = bat.position();
    const auto paddleSize = bat.size();
    const auto ballSize = sphere.size();
    sphere.setPosition({paddlePos.x + (paddleSize.x - ballSize.x) * 0.5f, paddlePos.y - ballSize.y});
    sphere.setVelocity({});
    sphere.setStuck(true);
}

void World::stickBallToPaddle()
{
    if(spheres.empty())
    {
        spheres.emplace_back(sf::Vector2f{0.f, 0.f}, art.ball);
    }

    stickBall(spheres.front());
}

void World::bounceWalls(Ball& sphere)
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

void World::bouncePaddle(Ball& sphere)
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

void World::bounceBricks(Ball& sphere)
{
    for(std::size_t i = 0; i < bricks.size(); ++i)
    {
        auto& brick = bricks[i];
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
        spawnDonorCapsule(i);

        const auto brickPos = brick.position();
        const auto brickSize = brick.size();
        const auto center = sphere.center();
        const float nearestX = std::clamp(center.x, brickPos.x, brickPos.x + brickSize.x);
        const float nearestY = std::clamp(center.y, brickPos.y, brickPos.y + brickSize.y);
        float nx = center.x - nearestX;
        float ny = center.y - nearestY;

        if(nx == 0.f && ny == 0.f)
        {
            const auto brickCenter = brickPos + brickSize * 0.5f;
            nx = center.x - brickCenter.x;
            ny = center.y - brickCenter.y;
        }

        auto vel = sphere.velocity();
        if(std::abs(nx) > std::abs(ny))
        {
            vel.x = std::copysign(std::abs(vel.x), nx);
        }
        else
        {
            vel.y = std::copysign(std::abs(vel.y), ny);
        }
        sphere.setVelocity(vel);
        return;
    }
}

void World::missCheck()
{
    if(spheres.empty())
    {
        return;
    }

    const bool allFallen = std::all_of(spheres.begin(), spheres.end(), offScreen);
    if(allFallen)
    {
        if(remaining > 0)
        {
            --remaining;
        }

        if(remaining > 0)
        {
            spheres.clear();
            spheres.emplace_back(sf::Vector2f{0.f, 0.f}, art.ball);
            stickBallToPaddle();
        }
        return;
    }

    std::erase_if(spheres, offScreen);
}

void World::spawnDonorCapsule(std::size_t index)
{
    if(index % 4 != 0)
    {
        return;
    }

    const auto kind = static_cast<PowerUpKind>((index / 4) % 4);
    capsules.emplace_back(bricks[index].position(), kind);
}

void World::updatePowerUps(sf::Time tick)
{
    for(auto& capsule: capsules)
    {
        capsule.fixedUpdate(tick);
    }

    std::erase_if(capsules, [](const PowerUp& capsule) { return !capsule.alive(); });

    std::size_t i = 0;
    while(i < capsules.size())
    {
        if(aabbOverlap(capsules[i].position(), capsules[i].size(), bat.position(), bat.size()))
        {
            const auto kind = capsules[i].kind();
            capsules.erase(capsules.begin() + static_cast<std::ptrdiff_t>(i));
            applyPowerUp(kind);
        }
        else
        {
            ++i;
        }
    }
}

void World::tickTimedEffect()
{
    if(timedRemaining == 0)
    {
        return;
    }

    --timedRemaining;
    if(timedRemaining == 0)
    {
        expireTimedEffect();
    }
}

void World::expireTimedEffect()
{
    if(!timedEffect.has_value())
    {
        timedRemaining = 0;
        return;
    }

    if(*timedEffect == PowerUpKind::Wide)
    {
        bat.setDisplayWidth(defaultPaddleWidth);
    }
    else if(*timedEffect == PowerUpKind::Slow && slowActive)
    {
        for(auto& sphere: spheres)
        {
            if(!offScreen(sphere))
            {
                sphere.setVelocity(sphere.velocity() / slowFactor);
            }
        }
        slowActive = false;
    }

    timedEffect.reset();
    timedRemaining = 0;
}

void World::applyMultiBall()
{
    if(spheres.empty())
    {
        return;
    }

    const auto& first = spheres.front();
    const auto pos = first.position();
    sf::Vector2f leftVel;
    sf::Vector2f rightVel;
    if(first.stuck())
    {
        leftVel = {launchX - multiSpread, launchY};
        rightVel = {launchX + multiSpread, launchY};
        if(slowActive)
        {
            leftVel *= slowFactor;
            rightVel *= slowFactor;
        }
    }
    else
    {
        const auto vel = first.velocity();
        leftVel = {vel.x - multiSpread, vel.y};
        rightVel = {vel.x + multiSpread, vel.y};
    }

    spheres.emplace_back(pos, art.ball);
    spheres.back().setStuck(false);
    spheres.back().setVelocity(leftVel);
    spheres.emplace_back(pos, art.ball);
    spheres.back().setStuck(false);
    spheres.back().setVelocity(rightVel);
}
