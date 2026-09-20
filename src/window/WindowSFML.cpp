/* Created by kblaszki */

#include "WindowSFML.hpp"

#include <Game.hpp>

WindowSFML::WindowSFML()
    : m_window{sf::VideoMode{Game::DESIGN_SIZE}, "Business game"}
{
    m_window.setFramerateLimit(60);
    m_window.setKeyRepeatEnabled(false);
}

bool WindowSFML::isOpen() const
{
    return m_window.isOpen();
}

void WindowSFML::close()
{
    m_window.close();
}

void WindowSFML::clear(const sf::Color& color)
{
    m_window.clear(color);
}

void WindowSFML::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
    m_window.draw(drawable, states);
}

void WindowSFML::display()
{
    m_window.display();
}

std::optional<sf::Event> WindowSFML::pollEvent()
{
    return m_window.pollEvent();
}

void WindowSFML::setView(const sf::View& view)
{
    m_window.setView(view);
}

sf::Vector2u WindowSFML::getSize() const
{
    return m_window.getSize();
}
