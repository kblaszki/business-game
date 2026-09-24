#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

struct BreakoutArt
{
    BreakoutArt();

    sf::Texture brick;
    sf::Texture paddle;
    sf::Texture ball;
    sf::Texture backdrop;
    sf::Sprite background;
};
