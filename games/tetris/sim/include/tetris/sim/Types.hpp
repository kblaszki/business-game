#pragma once

#include <array>
#include <cstdint>
#include <optional>

namespace sgl::tetris
{

enum class PieceType : uint8_t
{
    I,
    O,
    T,
    S,
    Z,
    J,
    L
};

enum class Rotation : uint8_t
{
    Spawn,
    Right,
    Reverse,
    Left
};

enum class Turn : uint8_t
{
    Cw,
    Ccw
};

struct Coord
{
    int x{};
    int y{};

    constexpr auto operator<=>(const Coord&) const = default;

    [[nodiscard]] constexpr Coord operator+(Coord o) const
    {
        return {x + o.x, y + o.y};
    }
};

using Cells = std::array<Coord, 4>;
using Tile = std::optional<PieceType>;

} // namespace sgl::tetris
