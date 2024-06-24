/* Created by kblaszki */
#pragma once

#include <managers/EventManagers.hpp>
#include <window/DrawerI.hpp>

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player(EventManagers& eventManagers);

    void draw(DrawerI& drawer) const;

private:
    sf::ConvexShape shape{};
};