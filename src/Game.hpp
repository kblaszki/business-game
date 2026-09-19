/* Created by kblaszki */
#pragma once

#include <SFML/System/Vector2.hpp>

class Game
{
public:
    static constexpr sf::Vector2u DESIGN_SIZE{1280u, 720u};

    void run();
};
