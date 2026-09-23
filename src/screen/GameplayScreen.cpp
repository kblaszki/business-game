#include "GameplayScreen.hpp"

#include <screen/ScreenStack.hpp>
#include <world/LevelDescriptor.hpp>

GameplayScreen::GameplayScreen(ScreenStack& screens, LevelId id)
    : screens{screens}
    , simulated{makeWorld(levelDescriptor(id))}
{
}

bool GameplayScreen::handleEvent(const sf::Event&)
{
    return false;
}

bool GameplayScreen::handleAction(Action action)
{
    if(action == Action::Pause)
    {
        screens.requestPauseOverlay();
        return true;
    }

    return false;
}

void GameplayScreen::update(sf::Time dt)
{
    simulated.fixedUpdate(dt);
}

void GameplayScreen::draw(DrawerI& drawer)
{
    simulated.draw(drawer);
}

bool GameplayScreen::blocksUpdate() const
{
    return true;
}

bool GameplayScreen::blocksDraw() const
{
    return true;
}

bool GameplayScreen::acceptsPauseOverlay() const
{
    return true;
}

std::uint32_t GameplayScreen::tickCount() const
{
    return simulated.tickCount();
}

const World& GameplayScreen::world() const
{
    return simulated;
}
