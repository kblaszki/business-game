/* Created by kblaszki */
#pragma once

#include "ScreenI.hpp"

#include <managers/EventManagers.hpp>
#include <window/ScreenRendererI.hpp>

#include <entities/Player.hpp>

class MenuScreen : public ScreenI
{
public:
    MenuScreen(EventManagers& eventManagers, ScreenRendererI& screenRenderer);

    void update() override;
    void display() override;

private:
    Player player;
    ScreenRendererI& screenRenderer;
};