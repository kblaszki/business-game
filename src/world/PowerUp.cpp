#include "PowerUp.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <Game.hpp>
#include <time/FixedTimestep.hpp>

namespace
{
constexpr float fallSpeed{180.f};

sf::Color tintFor(PowerUpKind kind)
{
    switch(kind)
    {
        case PowerUpKind::Wide:
            return {240, 210, 90};
        case PowerUpKind::MultiBall:
            return {80, 200, 230};
        case PowerUpKind::Slow:
            return {90, 140, 240};
        case PowerUpKind::ExtraLife:
            return {80, 200, 90};
    }
    return sf::Color::White;
}

sf::Image makeCapsule(sf::Color tint)
{
    constexpr unsigned w = 32u;
    constexpr unsigned h = 16u;
    sf::Image image{{w, h}, sf::Color::Transparent};
    const float cx = 15.5f;
    const float cy = 7.5f;
    const float rx = 14.f;
    const float ry = 6.f;
    for(unsigned y = 0; y < h; ++y)
    {
        for(unsigned x = 0; x < w; ++x)
        {
            const float dx = (static_cast<float>(x) - cx) / rx;
            const float dy = (static_cast<float>(y) - cy) / ry;
            if(dx * dx + dy * dy <= 1.f)
            {
                const bool highlight = dy < -0.35f;
                image.setPixel({x, y}, highlight ? sf::Color{255, 255, 255, 220} : tint);
            }
        }
    }
    return image;
}

sf::Texture makeTexture(PowerUpKind kind)
{
    sf::Texture texture;
    if(!texture.loadFromImage(makeCapsule(tintFor(kind))))
    {
        return texture;
    }
    return texture;
}

const sf::Texture& sharedTexture(PowerUpKind kind)
{
    static const sf::Texture wide = makeTexture(PowerUpKind::Wide);
    static const sf::Texture multi = makeTexture(PowerUpKind::MultiBall);
    static const sf::Texture slow = makeTexture(PowerUpKind::Slow);
    static const sf::Texture extra = makeTexture(PowerUpKind::ExtraLife);
    switch(kind)
    {
        case PowerUpKind::Wide:
            return wide;
        case PowerUpKind::MultiBall:
            return multi;
        case PowerUpKind::Slow:
            return slow;
        case PowerUpKind::ExtraLife:
            return extra;
    }
    return wide;
}
} // namespace

PowerUp::PowerUp(sf::Vector2f position, PowerUpKind kind)
    : sprite{sharedTexture(kind)}
    , powerKind{kind}
{
    sprite.setPosition(position);
}

void PowerUp::fixedUpdate(sf::Time tick)
{
    if(!live)
    {
        return;
    }

    auto pos = sprite.getPosition();
    pos.y += (fallSpeed / 60.f) * (tick / FixedTimestep::tick);
    sprite.setPosition(pos);
    if(pos.y >= static_cast<float>(Game::DESIGN_SIZE.y))
    {
        live = false;
    }
}

void PowerUp::draw(DrawerI& drawer) const
{
    if(live)
    {
        drawer.draw(sprite);
    }
}

bool PowerUp::alive() const
{
    return live;
}

PowerUpKind PowerUp::kind() const
{
    return powerKind;
}

sf::Vector2f PowerUp::position() const
{
    return sprite.getPosition();
}

sf::Vector2f PowerUp::size() const
{
    return sprite.getLocalBounds().size;
}
