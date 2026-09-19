/* Created by kblaszki */
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

class ScreenStack;

class Game
{
public:
    static constexpr sf::Vector2u DESIGN_SIZE{1280u, 720u};

    void run();

private:
    bool handleWindowEvent(sf::RenderWindow& window, ScreenStack& stack, const sf::Event& event);
};
