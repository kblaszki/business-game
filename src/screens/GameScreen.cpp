/* Created by kblaszki */

#include "GameScreen.hpp"

#include <entities/Paddle.hpp>
#include <managers/KeyboardManagerI.hpp>

GameScreen::GameScreen(EventManagers& eventManagers,
                       ScreenRendererI& screenRenderer,
                       ScreenUpdaterI& /*screenUpdater*/,
                       ResourceManager& /*resources*/)
    : screenRenderer{screenRenderer}
{
    entities.emplace_back(std::make_unique<Paddle>(eventManagers.get<ManagerOf::Keyboard>()));
}

void GameScreen::update(float dt)
{
    for(auto& entity: entities)
    {
        entity->update(dt);
    }
}

void GameScreen::display()
{
    for(auto& entity: entities)
    {
        entity->draw(screenRenderer);
    }
}
