/* Created by kblaszki */

#include "MenuScreen.hpp"

MenuScreen::MenuScreen(EventManagers& eventManagers, ScreenRendererI& screenRenderer)
    : player{eventManagers}
    , screenRenderer{screenRenderer}
{
}

void MenuScreen::update()
{
    player.update();
}

void MenuScreen::display()
{
    screenRenderer.clear();
    player.draw(screenRenderer);
    screenRenderer.display();
}