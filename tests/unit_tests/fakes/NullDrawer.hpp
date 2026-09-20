/* Created by kblaszki */
#pragma once

#include <window/DrawerI.hpp>

class NullDrawer : public DrawerI
{
public:
    void draw(const sf::Drawable&, const sf::RenderStates& = sf::RenderStates::Default) override {}
};
