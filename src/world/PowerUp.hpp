#pragma once

#include <window/DrawerI.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

enum class PowerUpKind
{
    Wide,
    MultiBall,
    Slow,
    ExtraLife
};

class PowerUp
{
public:
    PowerUp(sf::Vector2f position, PowerUpKind kind);

    void fixedUpdate(sf::Time tick);
    void draw(DrawerI& drawer) const;
    [[nodiscard]] bool alive() const;
    [[nodiscard]] PowerUpKind kind() const;
    [[nodiscard]] sf::Vector2f position() const;
    [[nodiscard]] sf::Vector2f size() const;

private:
    sf::Texture texture;
    sf::Sprite sprite;
    PowerUpKind powerKind;
    bool live{true};
};
