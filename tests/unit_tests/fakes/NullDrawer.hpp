/* Created by kblaszki */
#pragma once

#include <IDrawer.hpp>

class NullDrawer : public IDrawer
{
public:
    void draw(const sf::Drawable&, const sf::RenderStates& = sf::RenderStates::Default) override {}
};
