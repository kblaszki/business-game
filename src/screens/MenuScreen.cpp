/* Created by kblaszki */

#include "MenuScreen.hpp"

#include "GameScreen.hpp"

#include <entities/Button.hpp>
#include <managers/GameExitManagerI.hpp>
#include <managers/MouseManagerI.hpp>

MenuScreen::MenuScreen(EventManagers& eventManagers,
                       ScreenRendererI& screenRenderer,
                       ScreenUpdaterI& screenUpdater,
                       ResourceManager& resources)
    : screenRenderer{screenRenderer}
{
    const sf::Font& font = resources.getFont("fonts/prototype.ttf");

    entities.emplace_back(std::make_unique<Button>(
        eventManagers.get<ManagerOf::Mouse>(), font, "Start", sf::Vector2f(100, 100), sf::Vector2f(100, 50), [&]() {
            screenUpdater.replaceScreen(
                std::make_unique<GameScreen>(eventManagers, screenRenderer, screenUpdater, resources));
        }));
    entities.emplace_back(std::make_unique<Button>(
        eventManagers.get<ManagerOf::Mouse>(), font, "Exit", sf::Vector2f(100, 300), sf::Vector2f(100, 50), [&]() {
            eventManagers.get<ManagerOf::GameExit>().close();
        }));
}

void MenuScreen::update(float dt)
{
    for(auto& entity: entities)
    {
        entity->update(dt);
    }
}

void MenuScreen::display()
{
    for(auto& entity: entities)
    {
        entity->draw(screenRenderer);
    }
}
