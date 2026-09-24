#pragma once

#include <cstdint>
#include <eng/core/Color.hpp>
#include <eng/core/Rect.hpp>
#include <eng/core/Vec2.hpp>
#include <eng/render/AssetIds.hpp>
#include <string>
#include <variant>

namespace eng
{

enum class Layer : std::uint8_t
{
    Background,
    World,
    Actors,
    Hud,
    Overlay
};

struct SortKey
{
    Layer layer{};
    float depth{};
    std::uint32_t seq{};

    auto operator<=>(const SortKey&) const = default;
};

struct SpriteCmd
{
    TextureId texture;
    Rect<float> source;
    Vec2f position;
    Vec2f scale{1.f, 1.f};
    Color tint{255, 255, 255, 255};
};

struct RectCmd
{
    Rect<float> rect;
    Color fill;
    Color outline{0, 0, 0, 0};
    float outlineThickness{};
};

enum class Anchor
{
    TopLeft,
    Center
};

struct TextCmd
{
    FontId font;
    std::string text;
    unsigned size;
    Vec2f position;
    Color color;
    Anchor anchor{Anchor::TopLeft};
};

using DrawCommand = std::variant<SpriteCmd, RectCmd, TextCmd>;

struct Entry
{
    SortKey key;
    DrawCommand command;
};

} // namespace eng
