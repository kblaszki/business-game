#include "GameObject.hpp"

#include <Game.hpp>
#include <time/FixedTimestep.hpp>

GameObject::GameObject(sf::Vector2f position, sf::Vector2f velocity)
    : shape{{40.f, 40.f}}
    , velocity{velocity}
{
    shape.setFillColor(sf::Color{220, 80, 60});
    shape.setPosition(position);
}

void GameObject::fixedUpdate(sf::Time tick)
{
    auto pos = shape.getPosition();
    pos += (velocity / 60.f) * (tick / FixedTimestep::tick);

    const auto width = static_cast<float>(Game::DESIGN_SIZE.x);
    const auto height = static_cast<float>(Game::DESIGN_SIZE.y);

    while(pos.x >= width)
    {
        pos.x -= width;
    }
    while(pos.x < 0.f)
    {
        pos.x += width;
    }
    while(pos.y >= height)
    {
        pos.y -= height;
    }
    while(pos.y < 0.f)
    {
        pos.y += height;
    }

    shape.setPosition(pos);
}

void GameObject::draw(DrawerI& drawer) const
{
    drawer.draw(shape);
}

sf::Vector2f GameObject::position() const
{
    return shape.getPosition();
}
