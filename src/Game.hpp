#pragma once

#include <window/WindowI.hpp>

#include <SFML/System/Vector2.hpp>

class Game
{
public:
    static const sf::Vector2u DESIGN_SIZE;

    explicit Game(WindowI& window);

    void run();

private:
    WindowI& window;
};
