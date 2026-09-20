/* Created by kblaszki */
#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

class NullRenderTarget : public sf::RenderTarget
{
public:
    sf::Vector2u getSize() const override
    {
        return {1u, 1u};
    }

    bool setActive(bool) override
    {
        return false;
    }
};
