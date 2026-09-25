#pragma once

#include <optional>
#include <tetris/sim/Grid.hpp>
#include <tetris/sim/Shapes.hpp>
#include <tetris/sim/Types.hpp>

namespace sgl::tetris
{

struct Piece
{
    PieceType type{};
    Rotation rotation{Rotation::Spawn};
    Coord origin{};
};

[[nodiscard]] constexpr Cells cells(const Piece& piece)
{
    Cells result = shape(piece.type, piece.rotation);
    for(Coord& c: result)
    {
        c = c + piece.origin;
    }
    return result;
}

[[nodiscard]] Piece spawnPiece(PieceType type);
[[nodiscard]] std::optional<Piece> tryShift(const Grid& grid, Piece piece, Coord delta);
[[nodiscard]] std::optional<Piece> tryRotate(const Grid& grid, Piece piece, Turn turn);
[[nodiscard]] Piece dropToFloor(const Grid& grid, Piece piece);

} // namespace sgl::tetris
