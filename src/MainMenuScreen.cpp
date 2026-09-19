/* Created by kblaszki */

#include "MainMenuScreen.hpp"

#include "GameplayScreen.hpp"
#include "ScreenStack.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <memory>

MainMenuScreen::MainMenuScreen(ScreenStack& stack)
    : m_stack(stack)
{
}

bool MainMenuScreen::handleEvent(const sf::Event&)
{
    return false;
}

bool MainMenuScreen::handleAction(Action action)
{
    if(action == Action::Confirm)
    {
        m_stack.requestReplace(std::make_unique<GameplayScreen>(m_stack, LevelId::Arkanoid));
        return true;
    }

    return false;
}

void MainMenuScreen::update(sf::Time) {}

void MainMenuScreen::draw(sf::RenderTarget& target)
{
    sf::RectangleShape panel{{640.f, 360.f}};
    panel.setPosition({320.f, 180.f});
    panel.setFillColor(sf::Color{40, 70, 150});
    target.draw(panel);
}

bool MainMenuScreen::blocksUpdate() const
{
    return true;
}

bool MainMenuScreen::blocksDraw() const
{
    return true;
}
