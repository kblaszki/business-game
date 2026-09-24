#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <eng/core/Image.hpp>
#include <eng/render/AssetIds.hpp>
#include <eng/resources/ResourceCache.hpp>
#include <eng/resources/ResourceError.hpp>

#include <expected>
#include <filesystem>
#include <string_view>

namespace eng::sfml
{

class SfmlAssets
{
public:
    [[nodiscard]] std::expected<eng::TextureId, eng::ResourceError> texture(std::string_view key, const eng::Image& image);
    [[nodiscard]] std::expected<eng::FontId, eng::ResourceError> font(std::string_view key, const std::filesystem::path& path);

    [[nodiscard]] const sf::Texture* get(eng::TextureId id) const;
    [[nodiscard]] const sf::Font* get(eng::FontId id) const;

private:
    eng::ResourceCache<eng::TextureId, sf::Texture> textures_{};
    eng::ResourceCache<eng::FontId, sf::Font> fonts_{};
};

} // namespace eng::sfml
