/* Created by kblaszki */
#include "ScreenController.hpp"

#include <screens/MenuScreen.hpp>

ScreenController::ScreenController(EventManagers& eventManagers, ScreenRendererI& screenRenderer)
    : currentScreen{std::make_unique<MenuScreen>(eventManagers, screenRenderer)}
    , eventManagers{eventManagers}
    , screenRenderer{screenRenderer}
{
}

void ScreenController::update()
{
    currentScreen->update();
}

void ScreenController::display()
{
    currentScreen->display();
}
