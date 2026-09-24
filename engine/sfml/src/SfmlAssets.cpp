#include <eng/sfml/SfmlAssets.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>

#include <cstdint>

namespace eng::sfml
{
namespace
{

sf::Color toSfColor(eng::Color color)
{
    return sf::Color{color.r, color.g, color.b, color.a};
}

std::expected<sf::Texture, eng::ResourceError> loadTexture(const eng::Image& image)
{
    if(image.width == 0 || image.height == 0
       || image.pixels.size() != static_cast<std::size_t>(image.width) * static_cast<std::size_t>(image.height))
    {
        return std::unexpected(eng::ResourceError::DecodeFailed);
    }

    sf::Image sfImage{sf::Vector2u{image.width, image.height}, sf::Color::Transparent};
    for(std::uint32_t y = 0; y < image.height; ++y)
    {
        for(std::uint32_t x = 0; x < image.width; ++x)
        {
            const eng::Color pixel =
                image.pixels[static_cast<std::size_t>(y) * static_cast<std::size_t>(image.width) + static_cast<std::size_t>(x)];
            sfImage.setPixel({x, y}, toSfColor(pixel));
        }
    }

    sf::Texture texture;
    if(!texture.loadFromImage(sfImage))
    {
        return std::unexpected(eng::ResourceError::DecodeFailed);
    }
    return texture;
}

std::expected<sf::Font, eng::ResourceError> loadFont(const std::filesystem::path& path)
{
    if(!std::filesystem::exists(path))
    {
        return std::unexpected(eng::ResourceError::NotFound);
    }

    sf::Font font;
    if(!font.openFromFile(path))
    {
        return std::unexpected(eng::ResourceError::DecodeFailed);
    }
    return font;
}

} // namespace

std::expected<eng::TextureId, eng::ResourceError> SfmlAssets::texture(std::string_view key, const eng::Image& image)
{
    return textures_.load(key, [&]() { return loadTexture(image); });
}

std::expected<eng::FontId, eng::ResourceError> SfmlAssets::font(std::string_view key, const std::filesystem::path& path)
{
    return fonts_.load(key, [&]() { return loadFont(path); });
}

const sf::Texture* SfmlAssets::get(eng::TextureId id) const
{
    return textures_.get(id);
}

const sf::Font* SfmlAssets::get(eng::FontId id) const
{
    return fonts_.get(id);
}

} // namespace eng::sfml
