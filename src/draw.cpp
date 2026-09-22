/* Created by kblaszki */

#include "Ball.hpp"
#include "Brick.hpp"
#include "Game.hpp"
#include "GameplayScreen.hpp"
#include "MainMenuScreen.hpp"
#include "Paddle.hpp"
#include "PauseScreen.hpp"
#include "World.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

void Paddle::draw(sf::RenderTarget& target) const
{
    sf::RectangleShape shape{{WIDTH, HEIGHT}};
    shape.setPosition(m_position);
    shape.setFillColor(sf::Color::Green);
    target.draw(shape);
}

void Ball::draw(sf::RenderTarget& target) const
{
    sf::CircleShape shape{RADIUS};
    shape.setOrigin({RADIUS, RADIUS});
    shape.setPosition(m_position);
    shape.setFillColor(sf::Color::White);
    target.draw(shape);
}

void Brick::draw(sf::RenderTarget& target) const
{
    if(m_destroyed)
    {
        return;
    }

    sf::RectangleShape shape{{WIDTH, HEIGHT}};
    shape.setPosition(m_position);
    shape.setFillColor(m_fill);
    target.draw(shape);
}

void World::draw(sf::RenderTarget& target) const
{
    for(const auto& object: m_objects)
    {
        if(object->alive())
        {
            object->draw(target);
        }
    }
}

void GameplayScreen::drawLives(sf::RenderTarget& target) const
{
    for(int i = 0; i < m_lives; ++i)
    {
        sf::RectangleShape pip{{16.f, 16.f}};
        pip.setPosition({16.f + static_cast<float>(i) * 20.f, 12.f});
        pip.setFillColor(sf::Color::White);
        target.draw(pip);
    }
}

void GameplayScreen::draw(sf::RenderTarget& target)
{
    ++m_drawCount;
    m_world.draw(target);
    drawLives(target);
}

void PauseScreen::draw(sf::RenderTarget& target)
{
    sf::RectangleShape overlay{{static_cast<float>(Game::DESIGN_SIZE.x), static_cast<float>(Game::DESIGN_SIZE.y)}};
    overlay.setFillColor(sf::Color{0, 0, 0, 140});
    target.draw(overlay);
}

namespace
{
void drawButton(sf::RenderTarget& target, const sf::FloatRect& rect, sf::Color fill)
{
    sf::RectangleShape shape{rect.size};
    shape.setPosition(rect.position);
    shape.setFillColor(fill);
    target.draw(shape);
}
} // namespace

void MainMenuScreen::draw(sf::RenderTarget& target)
{
    sf::RectangleShape panel{{640.f, 360.f}};
    panel.setPosition({320.f, 180.f});
    panel.setFillColor(sf::Color{24, 32, 56});
    target.draw(panel);

    const sf::Color startFill = m_hover == Hover::Start ? sf::Color{80, 200, 110} : sf::Color{36, 130, 70};
    const sf::Color exitFill = m_hover == Hover::Exit ? sf::Color{210, 80, 80} : sf::Color{130, 40, 40};
    drawButton(target, startButton, startFill);
    drawButton(target, exitButton, exitFill);
}
