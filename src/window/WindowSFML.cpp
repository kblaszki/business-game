#include "WindowSFML.hpp"

WindowSFML::WindowSFML(sf::Vector2u size, const std::string& title)
    : window{sf::VideoMode{size}, title}
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

std::optional<sf::Event> WindowSFML::pollEvent()
{
    return window.pollEvent();
}

void WindowSFML::clear()
{
    window.clear();
}

void WindowSFML::display()
{
    window.display();
}

void WindowSFML::draw(const sf::Drawable& drawable)
{
    window.draw(drawable);
}
