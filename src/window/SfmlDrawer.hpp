/* Created by kblaszki */
#pragma once

#include "DrawerI.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

class SfmlDrawer : public DrawerI
{
public:
    explicit SfmlDrawer(sf::RenderTarget& target);

    void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default) override;

private:
    sf::RenderTarget& m_target;
};
