/* Created by kblaszki */

#include "MainMenuScreen.hpp"

#include "GameplayScreen.hpp"
#include "ScreenUpdaterI.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include <memory>

namespace
{
class MenuPanel : public EntityI
{
public:
    void fixedUpdate(sf::Time) override {}

    void draw(DrawerI& drawer) const override
    {
        sf::RectangleShape panel{{640.f, 360.f}};
        panel.setPosition({320.f, 180.f});
        panel.setFillColor(sf::Color{24, 32, 56});
        drawer.draw(panel);
    }
};

constexpr sf::Color startFill{36, 130, 70};
constexpr sf::Color startHoverFill{80, 200, 110};
constexpr sf::Color exitFill{130, 40, 40};
constexpr sf::Color exitHoverFill{210, 80, 80};
} // namespace

MainMenuScreen::MainMenuScreen(ScreenUpdaterI& stack)
    : m_stack(stack)
    , m_panel(std::make_unique<MenuPanel>())
    , m_start(startButton, startFill, startHoverFill)
    , m_exit(exitButton, exitFill, exitHoverFill)
{
}

void MainMenuScreen::setHoverFrom(sf::Vector2i pixel)
{
    m_start.setHovered(m_start.contains(pixel));
    m_exit.setHovered(m_exit.contains(pixel));
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
        if(m_start.hovered())
        {
            startGame();
            return true;
        }
        if(m_exit.hovered())
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
    m_panel->draw(drawer);
    m_start.draw(drawer);
    m_exit.draw(drawer);
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
    if(m_start.hovered())
    {
        return Hover::Start;
    }
    if(m_exit.hovered())
    {
        return Hover::Exit;
    }
    return Hover::None;
}
