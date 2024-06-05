/* Created by kblaszki */
#pragma once

#include <SFML/Graphics.hpp>

struct EventCollectorI
{
    virtual ~EventCollectorI() = default;
    virtual bool pollEvent(sf::Event& event) = 0;
};

struct WindowI : public EventCollectorI
{
    virtual ~WindowI() = default;
    virtual bool isOpen() = 0;
    virtual void close() = 0;
    virtual void clear(const sf::Color& color=sf::Color::Black) = 0;
    virtual void draw(const sf::Drawable &drawable, const sf::RenderStates& states=sf::RenderStates::Default) = 0;
    virtual void display() = 0;
};