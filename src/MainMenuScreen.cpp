/* Created by kblaszki */

#include "MainMenuScreen.hpp"

#include "GameplayScreen.hpp"
#include "ScreenStack.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include <memory>

namespace
{
bool contains(const sf::FloatRect& rect, sf::Vector2i pixel)
{
    return rect.contains(sf::Vector2f{static_cast<float>(pixel.x), static_cast<float>(pixel.y)});
}
} // namespace

MainMenuScreen::MainMenuScreen(ScreenStack& stack)
    : m_stack(stack)
{
}

MainMenuScreen::Hover MainMenuScreen::hitTest(sf::Vector2i pixel)
{
    if(contains(startButton, pixel))
    {
        return Hover::Start;
    }
    if(contains(exitButton, pixel))
    {
        return Hover::Exit;
    }
    return Hover::None;
}

void MainMenuScreen::setHoverFrom(sf::Vector2i pixel)
{
    m_hover = hitTest(pixel);
}

void MainMenuScreen::startGame()
{
    m_stack.requestReplace(std::make_unique<GameplayScreen>(m_stack, LevelId::Arkanoid));
}

bool MainMenuScreen::handleEvent(const sf::Event& event)
{
    if(const auto* const moved = event.getIf<sf::Event::MouseMoved>())
    {
        setHoverFrom(moved->position);
        return true;
    }

    if(const auto* const pressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if(pressed->button != sf::Mouse::Button::Left)
        {
            return false;
        }

        setHoverFrom(pressed->position);
        if(m_hover == Hover::Start)
        {
            startGame();
            return true;
        }
        if(m_hover == Hover::Exit)
        {
            m_stack.requestClose();
            return true;
        }
        return true;
    }

    return false;
}

bool MainMenuScreen::handleAction(Action action)
{
    if(action == Action::Confirm)
    {
        startGame();
        return true;
    }

    return false;
}

void MainMenuScreen::update(sf::Time) {}

bool MainMenuScreen::blocksUpdate() const
{
    return true;
}

bool MainMenuScreen::blocksDraw() const
{
    return true;
}

MainMenuScreen::Hover MainMenuScreen::hover() const
{
    return m_hover;
}
