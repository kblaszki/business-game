/* Created by kblaszki */

#include "MenuScreen.hpp"

#include <managers/KeyboardManagerI.hpp>
#include <managers/MouseManagerI.hpp>

MenuScreen::MenuScreen(EventManagers& eventManagers, ScreenRendererI& screenRenderer)
    : player{eventManagers.get<EventManagerOf::Keyboard>()}
    , buttonStart{eventManagers.get<EventManagerOf::Mouse>(),
                  "Start",
                  {100, 100},
                  {100, 50},
                  sf::Color::Red,
                  sf::Color::Green}
    , buttonExit{eventManagers.get<EventManagerOf::Mouse>(),
                 "Exit",
                 {100, 300},
                 {100, 50},
                 sf::Color::Red,
                 sf::Color::Green}
    , screenRenderer{screenRenderer}
{
}

void MenuScreen::update()
{
    buttonStart.update();
    buttonExit.update();
    player.update();
}

void MenuScreen::display()
{
    screenRenderer.clear();
    buttonStart.draw(screenRenderer);
    buttonExit.draw(screenRenderer);
    player.draw(screenRenderer);
    screenRenderer.display();
}