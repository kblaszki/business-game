#pragma once

#include <cstdint>
#include <optional>
#include <sgl/core/Color.hpp>
#include <vector>

namespace sgl
{

struct Image
{
    std::uint32_t width{};
    std::uint32_t height{};
    std::vector<Color> pixels;

    static Image filled(std::uint32_t w, std::uint32_t h, Color c)
    {
        Image image;
        image.width = w;
        image.height = h;
        image.pixels.assign(static_cast<std::size_t>(w) * static_cast<std::size_t>(h), c);
        return image;
    }

    [[nodiscard]] std::optional<Color> at(std::uint32_t x, std::uint32_t y) const
    {
        if(x >= width || y >= height)
        {
            return std::nullopt;
        }
        return pixels[static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x)];
    }

    bool set(std::uint32_t x, std::uint32_t y, Color c)
    {
        if(x >= width || y >= height)
        {
            return false;
        }
        pixels[static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x)] = c;
        return true;
    }
};

} // namespace sgl
