/* Created by kblaszki */

#include "GameScreen.hpp"

#include <entities/Paddle.hpp>
#include <managers/KeyboardManagerI.hpp>

GameScreen::GameScreen(EventManagers& eventManagers, ScreenRendererI& screenRenderer, ScreenUpdaterI& /*screenUpdater*/)
    : screenRenderer{screenRenderer}
{
    entities.emplace_back(std::make_unique<Paddle>(eventManagers.get<ManagerOf::Keyboard>()));
}

void GameScreen::update()
{
    for(auto& entity: entities)
    {
        entity->update();
    }
}

void GameScreen::display()
{
    screenRenderer.clear();
    for(auto& entity: entities)
    {
        entity->draw(screenRenderer);
    }
    screenRenderer.display();
}