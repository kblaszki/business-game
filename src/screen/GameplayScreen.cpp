#include "GameplayScreen.hpp"

#include <screen/ScreenStack.hpp>

GameplayScreen::GameplayScreen(ScreenStack& screens)
    : screens{screens}
    , dummy{{40.f, 40.f}}
{
    dummy.setFillColor(sf::Color{220, 80, 60});
    dummy.setPosition({0.f, 340.f});
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

void GameplayScreen::update(sf::Time)
{
    ++ticks;
}

void GameplayScreen::draw(DrawerI& drawer)
{
    drawer.draw(dummy);
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
    return ticks;
}
