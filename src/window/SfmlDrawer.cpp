/* Created by kblaszki */

#include "SfmlDrawer.hpp"

SfmlDrawer::SfmlDrawer(sf::RenderTarget& target)
    : m_target(target)
{
}

void SfmlDrawer::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
    m_target.draw(drawable, states);
}
