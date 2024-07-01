/* Created by kblaszki */
#pragma once

#include "ScreenI.hpp"

#include <entities/Button.hpp>
#include <entities/Player.hpp>
#include <managers/EventManagers.hpp>
#include <window/ScreenRendererI.hpp>

class MenuScreen : public ScreenI
{
public:
    MenuScreen(EventManagers& eventManagers, ScreenRendererI& screenRenderer);

    void update() override;
    void display() override;

private:
    Player player;
    Button buttonStart;
    Button buttonExit;
    ScreenRendererI& screenRenderer;
};