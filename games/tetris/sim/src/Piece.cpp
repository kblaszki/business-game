#include <tetris/sim/Piece.hpp>
#include <tetris/sim/Srs.hpp>

namespace sgl::tetris
{

Piece spawnPiece(PieceType type)
{
    const int n = boxSize(type);
    return Piece{.type = type, .rotation = Rotation::Spawn, .origin = Coord{3, 21 - (n - 1)}};
}

std::optional<Piece> tryShift(const Grid& grid, Piece piece, Coord delta)
{
    piece.origin = piece.origin + delta;
    const Cells c = cells(piece);
    if(grid.fits(c))
    {
        return piece;
    }
    return std::nullopt;
}

std::optional<Piece> tryRotate(const Grid& grid, Piece piece, Turn turn)
{
    if(piece.type == PieceType::O)
    {
        piece.rotation = rotate(piece.rotation, turn);
        return piece;
    }

    const Rotation from = piece.rotation;
    const Rotation to = rotate(from, turn);
    const KickTable table = kicks(piece.type, from, turn);
    for(Coord kick: table)
    {
        Piece candidate{.type = piece.type, .rotation = to, .origin = piece.origin + kick};
        const Cells c = cells(candidate);
        if(grid.fits(c))
        {
            return candidate;
        }
    }
    return std::nullopt;
}

Piece dropToFloor(const Grid& grid, Piece piece)
{
    while(true)
    {
        const std::optional<Piece> next = tryShift(grid, piece, Coord{0, -1});
        if(!next.has_value())
        {
            return piece;
        }
        piece = *next;
    }
}

} // namespace sgl::tetris
