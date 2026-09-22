#pragma once

#include <SFML/Window/Event.hpp>

#include <optional>

class WindowI
{
public:
    virtual ~WindowI() = default;

    virtual bool isOpen() const = 0;
    virtual void close() = 0;
    virtual std::optional<sf::Event> pollEvent() = 0;
    virtual void clear() = 0;
    virtual void display() = 0;
};
