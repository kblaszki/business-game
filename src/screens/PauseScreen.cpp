/* Created by kblaszki */

#include "PauseScreen.hpp"

#include "MainMenuScreen.hpp"
#include "ScreenUpdaterI.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <Game.hpp>
#include <memory>

PauseScreen::PauseScreen(ScreenUpdaterI& stack)
    : m_stack(stack)
{
}

bool PauseScreen::handleEvent(const sf::Event&)
{
    return false;
}

bool PauseScreen::handleAction(Action action)
{
    if(action == Action::Pause || action == Action::Cancel)
    {
        m_stack.requestPop();
        return true;
    }

    if(action == Action::Confirm)
    {
        m_stack.requestPop();
        m_stack.requestReplace(std::make_unique<MainMenuScreen>(m_stack));
        return true;
    }

    return false;
}

void PauseScreen::update(sf::Time) {}

void PauseScreen::draw(DrawerI& drawer)
{
    sf::RectangleShape overlay{{static_cast<float>(Game::DESIGN_SIZE.x), static_cast<float>(Game::DESIGN_SIZE.y)}};
    overlay.setFillColor(sf::Color{0, 0, 0, 140});
    drawer.draw(overlay);
}

bool PauseScreen::blocksUpdate() const
{
    return true;
}

bool PauseScreen::blocksDraw() const
{
    return false;
}
