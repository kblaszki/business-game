#include "MainMenuScreen.hpp"

#include <SFML/Window/Keyboard.hpp>

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

bool MainMenuScreen::handleEvent(const sf::Event& event)
{
    if(const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if(key->code == sf::Keyboard::Key::Enter)
        {
            screens.replace(std::make_unique<GameplayScreen>(screens));
            return true;
        }
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
