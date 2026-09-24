#include "BreakoutArt.hpp"

#include <SFML/Graphics/Image.hpp>

#include <Game.hpp>
#include <cstdint>

namespace
{
sf::Texture textureFrom(const sf::Image& image)
{
    sf::Texture texture;
    if(!texture.loadFromImage(image))
    {
        return texture;
    }
    return texture;
}

sf::Image fill(sf::Vector2u size, sf::Color color)
{
    sf::Image image{size, color};
    return image;
}

void box(sf::Image& image, unsigned x0, unsigned y0, unsigned x1, unsigned y1, sf::Color color)
{
    for(unsigned y = y0; y < y1; ++y)
    {
        for(unsigned x = x0; x < x1; ++x)
        {
            image.setPixel({x, y}, color);
        }
    }
}
} // namespace

BreakoutArt::BreakoutArt()
    : brick{textureFrom([] {
        sf::Image image = fill({110u, 28u}, sf::Color{40, 20, 30});
        box(image, 2, 2, 108, 26, sf::Color::White);
        box(image, 2, 2, 108, 6, sf::Color{255, 255, 255, 80});
        return image;
    }())}
    , paddle{textureFrom([] {
        sf::Image image = fill({120u, 20u}, sf::Color{20, 20, 28});
        box(image, 2, 2, 118, 18, sf::Color{240, 210, 90});
        box(image, 2, 2, 118, 6, sf::Color{255, 240, 160});
        return image;
    }())}
    , ball{textureFrom([] {
        sf::Image image = fill({16u, 16u}, sf::Color::Transparent);
        const int cx = 8;
        const int cy = 8;
        for(int y = 0; y < 16; ++y)
        {
            for(int x = 0; x < 16; ++x)
            {
                const int dx = x - cx;
                const int dy = y - cy;
                if(dx * dx + dy * dy <= 49)
                {
                    const bool highlight = dx + dy < -2;
                    image.setPixel({static_cast<unsigned>(x), static_cast<unsigned>(y)},
                                   highlight ? sf::Color{255, 240, 220} : sf::Color{230, 90, 70});
                }
            }
        }
        return image;
    }())}
    , backdrop{textureFrom([] {
        const auto w = Game::DESIGN_SIZE.x;
        const auto h = Game::DESIGN_SIZE.y;
        sf::Image image = fill({w, h}, sf::Color{12, 16, 32});
        for(unsigned y = 0; y < h; y += 24)
        {
            for(unsigned x = 0; x < w; ++x)
            {
                auto c = image.getPixel({x, y});
                c.r = static_cast<std::uint8_t>(c.r + 8);
                c.b = static_cast<std::uint8_t>(c.b + 12);
                image.setPixel({x, y}, c);
            }
        }
        return image;
    }())}
    , background{backdrop}
{
}
