/* Created by kblaszki */

#include "MenuScreen.hpp"

MenuScreen::MenuScreen(EventManagers& eventManagers, ScreenRendererI& screenRenderer)
    : player{eventManagers}
    , button{eventManagers, "Start", {100, 100}, {100, 50}, sf::Color::Red, sf::Color::Green}
    , screenRenderer{screenRenderer}
{
}

void MenuScreen::update()
{
    button.update();
    player.update();
}

void MenuScreen::display()
{
    screenRenderer.clear();
    button.draw(screenRenderer);
    player.draw(screenRenderer);
    screenRenderer.display();
}