#pragma once

#include <window/DrawerI.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

class ScreenI
{
public:
    virtual ~ScreenI() = default;

    virtual bool handleEvent(const sf::Event& event) = 0;
    virtual void update(sf::Time dt) = 0;
    virtual void draw(DrawerI& drawer) = 0;
    [[nodiscard]] virtual bool blocksUpdate() const = 0;
    [[nodiscard]] virtual bool blocksDraw() const = 0;
};
