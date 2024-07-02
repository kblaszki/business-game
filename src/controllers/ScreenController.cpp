/* Created by kblaszki */
#include "ScreenController.hpp"

#include <screens/MenuScreen.hpp>

ScreenController::ScreenController(EventManagers& eventManagers, ScreenRendererI& screenRenderer)
    : currentScreen{std::make_unique<MenuScreen>(eventManagers, screenRenderer, *this)}
    , eventManagers{eventManagers}
    , screenRenderer{screenRenderer}
{
}

void ScreenController::setScreen(std::unique_ptr<ScreenI>&& screen)
{
    newScreen = std::move(screen);
}

void ScreenController::update()
{
    if(newScreen)
    {
        currentScreen = std::move(newScreen);
        newScreen = nullptr;
    }
    currentScreen->update();
}

void ScreenController::display()
{
    currentScreen->display();
}
