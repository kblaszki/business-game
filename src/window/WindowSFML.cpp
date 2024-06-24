/* Created by kblaszki */

#include "WindowSFML.hpp"

WindowSFML::WindowSFML()
    : window{sf::VideoMode(1280, 720), "Business game", sf::Style::Default}
{
    window.setFramerateLimit(60);
}

bool WindowSFML::isOpen() const
{
    return window.isOpen();
}

void WindowSFML::close()
{
    window.close();
}

void WindowSFML::clear(const sf::Color& color)
{
    window.clear(color);
}

void WindowSFML::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
    window.draw(drawable, states);
}

void WindowSFML::display()
{
    window.display();
}

bool WindowSFML::pollEvent(sf::Event& event)
{
    return window.pollEvent(event);
}