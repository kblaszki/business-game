/* Created by kblaszki */

#include "WindowSFML.hpp"

namespace
{
sf::Vector2i toPixelApprox(sf::Vector2f world)
{
    return {static_cast<int>(world.x), static_cast<int>(world.y)};
}
} // namespace

WindowSFML::WindowSFML()
    : window{sf::VideoMode({1280, 720}), "Business game", sf::Style::Default}
{
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
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

std::optional<sf::Event> WindowSFML::pollEvent()
{
    auto event = window.pollEvent();
    if(event)
    {
        mapMouseEventToWorld(*event);
    }
    return event;
}

void WindowSFML::setView(const sf::View& view)
{
    window.setView(view);
}

sf::Vector2u WindowSFML::getSize() const
{
    return window.getSize();
}

void WindowSFML::mapMouseEventToWorld(sf::Event& event)
{
    if(auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
    {
        mouseMoved->position = toPixelApprox(window.mapPixelToCoords(mouseMoved->position));
    }
    else if(auto* buttonPressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        buttonPressed->position = toPixelApprox(window.mapPixelToCoords(buttonPressed->position));
    }
    else if(auto* buttonReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        buttonReleased->position = toPixelApprox(window.mapPixelToCoords(buttonReleased->position));
    }
    else if(auto* wheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>())
    {
        wheelScrolled->position = toPixelApprox(window.mapPixelToCoords(wheelScrolled->position));
    }
}
