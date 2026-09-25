#pragma once

#include <tetris/sim/Types.hpp>

namespace sgl::tetris
{

[[nodiscard]] constexpr int boxSize(PieceType type)
{
    switch(type)
    {
        case PieceType::I:
            return 4;
        case PieceType::O:
            return 2;
        case PieceType::T:
        case PieceType::S:
        case PieceType::Z:
        case PieceType::J:
        case PieceType::L:
            return 3;
    }
    return 3;
}

[[nodiscard]] constexpr Coord rotateCwInBox(Coord c, int n)
{
    return {c.y, n - 1 - c.x};
}

[[nodiscard]] constexpr Cells rotateCellsCw(Cells cells, int n)
{
    for(Coord& c: cells)
    {
        c = rotateCwInBox(c, n);
    }
    return cells;
}

[[nodiscard]] constexpr Cells spawnShape(PieceType type)
{
    switch(type)
    {
        case PieceType::T:
            return {Coord{0, 1}, Coord{1, 1}, Coord{2, 1}, Coord{1, 2}};
        case PieceType::J:
            return {Coord{0, 2}, Coord{0, 1}, Coord{1, 1}, Coord{2, 1}};
        case PieceType::L:
            return {Coord{2, 2}, Coord{0, 1}, Coord{1, 1}, Coord{2, 1}};
        case PieceType::S:
            return {Coord{1, 2}, Coord{2, 2}, Coord{0, 1}, Coord{1, 1}};
        case PieceType::Z:
            return {Coord{0, 2}, Coord{1, 2}, Coord{1, 1}, Coord{2, 1}};
        case PieceType::I:
            return {Coord{0, 2}, Coord{1, 2}, Coord{2, 2}, Coord{3, 2}};
        case PieceType::O:
            return {Coord{0, 0}, Coord{1, 0}, Coord{0, 1}, Coord{1, 1}};
    }
    return {};
}

[[nodiscard]] constexpr Cells shape(PieceType type, Rotation rotation)
{
    if(type == PieceType::O)
    {
        return spawnShape(PieceType::O);
    }

    const int n = boxSize(type);
    Cells cells = spawnShape(type);
    const int turns = static_cast<int>(rotation);
    for(int i = 0; i < turns; ++i)
    {
        cells = rotateCellsCw(cells, n);
    }
    return cells;
}

[[nodiscard]] constexpr bool cellsDistinct(const Cells& cells)
{
    for(std::size_t i = 0; i < cells.size(); ++i)
    {
        for(std::size_t j = i + 1; j < cells.size(); ++j)
        {
            if(cells[i] == cells[j])
            {
                return false;
            }
        }
    }
    return true;
}

[[nodiscard]] constexpr bool sameCells(Cells a, Cells b)
{
    for(Coord c: a)
    {
        bool found = false;
        for(Coord& d: b)
        {
            if(c == d)
            {
                d = Coord{-1, -1};
                found = true;
                break;
            }
        }
        if(!found)
        {
            return false;
        }
    }
    return true;
}

[[nodiscard]] constexpr bool fourCwReturnsSpawn(PieceType type)
{
    Cells cells = spawnShape(type);
    if(type != PieceType::O)
    {
        const int n = boxSize(type);
        for(int i = 0; i < 4; ++i)
        {
            cells = rotateCellsCw(cells, n);
        }
    }
    return sameCells(cells, spawnShape(type));
}

static_assert(cellsDistinct(shape(PieceType::I, Rotation::Spawn)));
static_assert(cellsDistinct(shape(PieceType::I, Rotation::Right)));
static_assert(cellsDistinct(shape(PieceType::I, Rotation::Reverse)));
static_assert(cellsDistinct(shape(PieceType::I, Rotation::Left)));
static_assert(cellsDistinct(shape(PieceType::O, Rotation::Spawn)));
static_assert(cellsDistinct(shape(PieceType::O, Rotation::Right)));
static_assert(cellsDistinct(shape(PieceType::O, Rotation::Reverse)));
static_assert(cellsDistinct(shape(PieceType::O, Rotation::Left)));
static_assert(cellsDistinct(shape(PieceType::T, Rotation::Spawn)));
static_assert(cellsDistinct(shape(PieceType::T, Rotation::Right)));
static_assert(cellsDistinct(shape(PieceType::T, Rotation::Reverse)));
static_assert(cellsDistinct(shape(PieceType::T, Rotation::Left)));
static_assert(cellsDistinct(shape(PieceType::S, Rotation::Spawn)));
static_assert(cellsDistinct(shape(PieceType::S, Rotation::Right)));
static_assert(cellsDistinct(shape(PieceType::S, Rotation::Reverse)));
static_assert(cellsDistinct(shape(PieceType::S, Rotation::Left)));
static_assert(cellsDistinct(shape(PieceType::Z, Rotation::Spawn)));
static_assert(cellsDistinct(shape(PieceType::Z, Rotation::Right)));
static_assert(cellsDistinct(shape(PieceType::Z, Rotation::Reverse)));
static_assert(cellsDistinct(shape(PieceType::Z, Rotation::Left)));
static_assert(cellsDistinct(shape(PieceType::J, Rotation::Spawn)));
static_assert(cellsDistinct(shape(PieceType::J, Rotation::Right)));
static_assert(cellsDistinct(shape(PieceType::J, Rotation::Reverse)));
static_assert(cellsDistinct(shape(PieceType::J, Rotation::Left)));
static_assert(cellsDistinct(shape(PieceType::L, Rotation::Spawn)));
static_assert(cellsDistinct(shape(PieceType::L, Rotation::Right)));
static_assert(cellsDistinct(shape(PieceType::L, Rotation::Reverse)));
static_assert(cellsDistinct(shape(PieceType::L, Rotation::Left)));

static_assert(fourCwReturnsSpawn(PieceType::I));
static_assert(fourCwReturnsSpawn(PieceType::O));
static_assert(fourCwReturnsSpawn(PieceType::T));
static_assert(fourCwReturnsSpawn(PieceType::S));
static_assert(fourCwReturnsSpawn(PieceType::Z));
static_assert(fourCwReturnsSpawn(PieceType::J));
static_assert(fourCwReturnsSpawn(PieceType::L));

} // namespace sgl::tetris
