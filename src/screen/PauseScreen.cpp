#include "PauseScreen.hpp"

#include <Game.hpp>
#include <memory>
#include <screen/MainMenuScreen.hpp>
#include <screen/ScreenStack.hpp>

PauseScreen::PauseScreen(ScreenStack& screens)
    : screens{screens}
    , dim{{static_cast<float>(Game::DESIGN_SIZE.x), static_cast<float>(Game::DESIGN_SIZE.y)}}
{
    dim.setFillColor(sf::Color{0, 0, 0, 140});
}

bool PauseScreen::handleEvent(const sf::Event&)
{
    return false;
}

bool PauseScreen::handleAction(Action action)
{
    if(action == Action::Pause || action == Action::Cancel)
    {
        screens.pop();
        return true;
    }

    if(action == Action::Confirm)
    {
        screens.pop();
        screens.replace(std::make_unique<MainMenuScreen>(screens));
        return true;
    }

    return false;
}

void PauseScreen::update(sf::Time) {}

void PauseScreen::draw(DrawerI& drawer)
{
    drawer.draw(dim);
}

bool PauseScreen::blocksUpdate() const
{
    return true;
}

bool PauseScreen::blocksDraw() const
{
    return false;
}

bool PauseScreen::isPauseOverlay() const
{
    return true;
}
