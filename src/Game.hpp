/* Created by kblaszki */
#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

class ScreenStack;
struct WindowI;

class Game
{
public:
    static constexpr sf::Vector2u DESIGN_SIZE{1280u, 720u};

    void run();
    void run(WindowI& window);

private:
    bool handleWindowEvent(WindowI& window, ScreenStack& stack, const sf::Event& event);
};
