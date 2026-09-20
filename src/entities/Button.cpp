/* Created by kblaszki */

#include "Button.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

Button::Button(sf::FloatRect bounds, sf::Color fill, sf::Color hoverFill)
    : m_bounds(bounds)
    , m_fill(fill)
    , m_hoverFill(hoverFill)
{
}

void Button::fixedUpdate(sf::Time) {}

void Button::draw(DrawerI& drawer) const
{
    sf::RectangleShape shape{m_bounds.size};
    shape.setPosition(m_bounds.position);
    shape.setFillColor(m_hovered ? m_hoverFill : m_fill);
    drawer.draw(shape);
}

bool Button::contains(sf::Vector2i pixel) const
{
    return m_bounds.contains(sf::Vector2f{static_cast<float>(pixel.x), static_cast<float>(pixel.y)});
}

void Button::setHovered(bool hovered)
{
    m_hovered = hovered;
}

bool Button::hovered() const
{
    return m_hovered;
}
