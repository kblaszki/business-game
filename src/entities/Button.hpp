/* Created by kblaszki */
#pragma once

#include "EntityI.hpp"
#include "HitTestI.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

class Button
    : public EntityI
    , public HitTestI
{
public:
    Button(sf::FloatRect bounds, sf::Color fill, sf::Color hoverFill);

    void fixedUpdate(sf::Time tick) override;
    void draw(DrawerI& drawer) const override;
    bool contains(sf::Vector2i pixel) const override;

    void setHovered(bool hovered);
    [[nodiscard]] bool hovered() const;

private:
    sf::FloatRect m_bounds{};
    sf::Color m_fill{};
    sf::Color m_hoverFill{};
    bool m_hovered{false};
};
