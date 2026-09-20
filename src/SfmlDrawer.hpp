/* Created by kblaszki */
#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

#include <IDrawer.hpp>

class SfmlDrawer : public IDrawer
{
public:
    explicit SfmlDrawer(sf::RenderTarget& target);

    void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default) override;

private:
    sf::RenderTarget& m_target;
};
