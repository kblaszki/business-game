/* Created by kblaszki */
#pragma once

#include "EventCollectorI.hpp"

#include <SFML/Graphics.hpp>
struct WindowI : public EventCollectorI
{
    virtual ~WindowI() = default;
    virtual bool isOpen() = 0;
    virtual void close() = 0;
    virtual void clear(const sf::Color& color = sf::Color::Black) = 0;
    virtual void
    draw(const sf::Drawable& drawable,
         const sf::RenderStates& states = sf::RenderStates::Default) = 0;
    virtual void display() = 0;
};