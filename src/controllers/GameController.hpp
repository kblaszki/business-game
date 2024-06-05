/* Created by kblaszki */
#pragma once

#include <core/WindowI.hpp>

#include <SFML/Graphics.hpp>
#include <memory>

class GameController
{
public:
    GameController(std::unique_ptr<WindowI>&& gameWindow);

    void run();
private:

    std::unique_ptr<WindowI> window;
};