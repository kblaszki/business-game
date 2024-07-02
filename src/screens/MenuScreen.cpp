/* Created by kblaszki */

#include "MenuScreen.hpp"

#include "GameScreen.hpp"

#include <entities/Button.hpp>
#include <managers/GameExitManagerI.hpp>
#include <managers/MouseManagerI.hpp>

MenuScreen::MenuScreen(EventManagers& eventManagers, ScreenRendererI& screenRenderer, ScreenUpdaterI& screenUpdater)
    : screenRenderer{screenRenderer}
{
    entities.emplace_back(std::make_unique<Button>(
        eventManagers.get<ManagerOf::Mouse>(), "Start", sf::Vector2f(100, 100), sf::Vector2f(100, 50), [&]() {
            screenUpdater.setScreen(std::make_unique<GameScreen>(eventManagers, screenRenderer, screenUpdater));
        }));
    entities.emplace_back(std::make_unique<Button>(
        eventManagers.get<ManagerOf::Mouse>(), "Exit", sf::Vector2f(100, 300), sf::Vector2f(100, 50), [&]() {
            eventManagers.get<ManagerOf::GameExit>().close();
        }));
}

void MenuScreen::update()
{
    for(auto& entity: entities)
    {
        entity->update();
    }
}

void MenuScreen::display()
{
    screenRenderer.clear();
    for(auto& entity: entities)
    {
        entity->draw(screenRenderer);
    }
    screenRenderer.display();
}