/* Created by kblaszki */
#pragma once

#include "WindowI.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

class GameController
{
public:
    GameController();

    void run();
private:

    std::unique_ptr<WindowI> window;
};