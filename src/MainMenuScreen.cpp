/* Created by kblaszki */

#include "MainMenuScreen.hpp"

#include "GameplayScreen.hpp"
#include "ScreenStack.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
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

void MainMenuScreen::draw(DrawerI& drawer)
{
    sf::RectangleShape panel{{640.f, 360.f}};
    panel.setPosition({320.f, 180.f});
    panel.setFillColor(sf::Color{24, 32, 56});
    drawer.draw(panel);

    const sf::Color startFill = m_hover == Hover::Start ? sf::Color{80, 200, 110} : sf::Color{36, 130, 70};
    const sf::Color exitFill = m_hover == Hover::Exit ? sf::Color{210, 80, 80} : sf::Color{130, 40, 40};

    sf::RectangleShape startShape{startButton.size};
    startShape.setPosition(startButton.position);
    startShape.setFillColor(startFill);
    drawer.draw(startShape);

    sf::RectangleShape exitShape{exitButton.size};
    exitShape.setPosition(exitButton.position);
    exitShape.setFillColor(exitFill);
    drawer.draw(exitShape);
}

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
