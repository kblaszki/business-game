#include "MainMenuScreen.hpp"

#include <Game.hpp>
#include <memory>
#include <screen/GameplayScreen.hpp>
#include <screen/ScreenStack.hpp>
#include <screen/UiFont.hpp>

namespace
{
void centerAt(sf::Text& text, float y)
{
    const auto bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    text.setPosition({static_cast<float>(Game::DESIGN_SIZE.x) / 2.f, y});
}
} // namespace

MainMenuScreen::MainMenuScreen(ScreenStack& screens)
    : screens{screens}
    , font{makeUiFont()}
    , startBar{{400.f, 80.f}}
    , title{font, "Business game", 42}
    , startHint{font, "Enter — start", 22}
    , quitHint{font, "Backspace — quit", 22}
{
    startBar.setFillColor(sf::Color{60, 120, 200});
    startBar.setPosition({(Game::DESIGN_SIZE.x - 400.f) / 2.f, (Game::DESIGN_SIZE.y - 80.f) / 2.f});

    title.setFillColor(sf::Color::White);
    startHint.setFillColor(sf::Color::White);
    quitHint.setFillColor(sf::Color::White);
    centerAt(title, 240.f);
    centerAt(startHint, 430.f);
    centerAt(quitHint, 470.f);
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

    if(action == Action::Cancel)
    {
        screens.requestClose();
        return true;
    }

    return false;
}

void MainMenuScreen::update(sf::Time) {}

void MainMenuScreen::draw(DrawerI& drawer)
{
    drawer.draw(startBar);
    drawer.draw(title);
    drawer.draw(startHint);
    drawer.draw(quitHint);
}

bool MainMenuScreen::blocksUpdate() const
{
    return true;
}

bool MainMenuScreen::blocksDraw() const
{
    return true;
}
