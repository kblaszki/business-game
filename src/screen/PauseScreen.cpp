#include "PauseScreen.hpp"

#include <Game.hpp>
#include <memory>
#include <screen/MainMenuScreen.hpp>
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

PauseScreen::PauseScreen(ScreenStack& screens)
    : screens{screens}
    , font{makeUiFont()}
    , dim{{static_cast<float>(Game::DESIGN_SIZE.x), static_cast<float>(Game::DESIGN_SIZE.y)}}
    , panel{{520.f, 240.f}}
    , title{font, "Paused", 42}
    , resumeHint{font, "Esc - resume", 22}
    , quitHint{font, "Enter - quit to menu", 22}
{
    dim.setFillColor(sf::Color{0, 0, 0, 150});
    panel.setFillColor(sf::Color{24, 28, 48, 230});
    panel.setPosition({380.f, 240.f});
    title.setFillColor(sf::Color{255, 230, 160});
    resumeHint.setFillColor(sf::Color::White);
    quitHint.setFillColor(sf::Color::White);
    centerAt(title, 300.f);
    centerAt(resumeHint, 360.f);
    centerAt(quitHint, 400.f);
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
    drawer.draw(panel);
    drawer.draw(title);
    drawer.draw(resumeHint);
    drawer.draw(quitHint);
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
