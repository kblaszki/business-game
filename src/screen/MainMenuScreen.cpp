#include "MainMenuScreen.hpp"

#include <Game.hpp>
#include <memory>
#include <screen/GameplayScreen.hpp>
#include <screen/ScreenStack.hpp>

MainMenuScreen::MainMenuScreen(ScreenStack& screens)
    : screens{screens}
    , startBar{{400.f, 80.f}}
{
    startBar.setFillColor(sf::Color{60, 120, 200});
    startBar.setPosition({(Game::DESIGN_SIZE.x - 400.f) / 2.f, (Game::DESIGN_SIZE.y - 80.f) / 2.f});
}

bool MainMenuScreen::handleEvent(const sf::Event&)
{
    return false;
}

bool MainMenuScreen::handleAction(Action action)
{
    if(action == Action::Confirm)
    {
        screens.replace(std::make_unique<GameplayScreen>(screens, LevelId::Sandbox));
        return true;
    }

    return false;
}

void MainMenuScreen::update(sf::Time) {}

void MainMenuScreen::draw(DrawerI& drawer)
{
    drawer.draw(startBar);
}

bool MainMenuScreen::blocksUpdate() const
{
    return true;
}

bool MainMenuScreen::blocksDraw() const
{
    return true;
}
