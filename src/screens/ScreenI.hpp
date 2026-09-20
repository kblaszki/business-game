/* Created by kblaszki */
#pragma once

#include <window/DrawerI.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <Action.hpp>

class ScreenI
{
public:
    virtual ~ScreenI() = default;

    virtual bool handleEvent(const sf::Event& event) = 0;
    virtual bool handleAction(Action action) = 0;
    virtual void update(sf::Time dt) = 0;
    virtual void draw(DrawerI& drawer) = 0;
    virtual bool blocksUpdate() const = 0;
    virtual bool blocksDraw() const = 0;
    virtual bool isGameplay() const
    {
        return false;
    }
    virtual bool isPauseOverlay() const
    {
        return false;
    }
};
