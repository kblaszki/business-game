#pragma once

#include <array>
#include <tetris/sim/Types.hpp>

namespace sgl::tetris
{

[[nodiscard]] constexpr Rotation rotate(Rotation rotation, Turn turn)
{
    if(turn == Turn::Cw)
    {
        switch(rotation)
        {
            case Rotation::Spawn:
                return Rotation::Right;
            case Rotation::Right:
                return Rotation::Reverse;
            case Rotation::Reverse:
                return Rotation::Left;
            case Rotation::Left:
                return Rotation::Spawn;
        }
    }
    switch(rotation)
    {
        case Rotation::Spawn:
            return Rotation::Left;
        case Rotation::Left:
            return Rotation::Reverse;
        case Rotation::Reverse:
            return Rotation::Right;
        case Rotation::Right:
            return Rotation::Spawn;
    }
    return Rotation::Spawn;
}

using KickTable = std::array<Coord, 5>;

[[nodiscard]] constexpr KickTable kicks(PieceType type, Rotation from, Turn turn)
{
    if(type == PieceType::O)
    {
        return {};
    }

    const Rotation to = rotate(from, turn);

    if(type == PieceType::I)
    {
        // from -> to
        if(from == Rotation::Spawn && to == Rotation::Right)
        {
            return {Coord{0, 0}, Coord{-2, 0}, Coord{1, 0}, Coord{-2, -1}, Coord{1, 2}};
        }
        if(from == Rotation::Right && to == Rotation::Spawn)
        {
            return {Coord{0, 0}, Coord{2, 0}, Coord{-1, 0}, Coord{2, 1}, Coord{-1, -2}};
        }
        if(from == Rotation::Right && to == Rotation::Reverse)
        {
            return {Coord{0, 0}, Coord{-1, 0}, Coord{2, 0}, Coord{-1, 2}, Coord{2, -1}};
        }
        if(from == Rotation::Reverse && to == Rotation::Right)
        {
            return {Coord{0, 0}, Coord{1, 0}, Coord{-2, 0}, Coord{1, -2}, Coord{-2, 1}};
        }
        if(from == Rotation::Reverse && to == Rotation::Left)
        {
            return {Coord{0, 0}, Coord{2, 0}, Coord{-1, 0}, Coord{2, 1}, Coord{-1, -2}};
        }
        if(from == Rotation::Left && to == Rotation::Reverse)
        {
            return {Coord{0, 0}, Coord{-2, 0}, Coord{1, 0}, Coord{-2, -1}, Coord{1, 2}};
        }
        if(from == Rotation::Left && to == Rotation::Spawn)
        {
            return {Coord{0, 0}, Coord{1, 0}, Coord{-2, 0}, Coord{1, -2}, Coord{-2, 1}};
        }
        if(from == Rotation::Spawn && to == Rotation::Left)
        {
            return {Coord{0, 0}, Coord{-1, 0}, Coord{2, 0}, Coord{-1, 2}, Coord{2, -1}};
        }
        return {};
    }

    // JLSTZ
    if(from == Rotation::Spawn && to == Rotation::Right)
    {
        return {Coord{0, 0}, Coord{-1, 0}, Coord{-1, 1}, Coord{0, -2}, Coord{-1, -2}};
    }
    if(from == Rotation::Right && to == Rotation::Spawn)
    {
        return {Coord{0, 0}, Coord{1, 0}, Coord{1, -1}, Coord{0, 2}, Coord{1, 2}};
    }
    if(from == Rotation::Right && to == Rotation::Reverse)
    {
        return {Coord{0, 0}, Coord{1, 0}, Coord{1, -1}, Coord{0, 2}, Coord{1, 2}};
    }
    if(from == Rotation::Reverse && to == Rotation::Right)
    {
        return {Coord{0, 0}, Coord{-1, 0}, Coord{-1, 1}, Coord{0, -2}, Coord{-1, -2}};
    }
    if(from == Rotation::Reverse && to == Rotation::Left)
    {
        return {Coord{0, 0}, Coord{1, 0}, Coord{1, 1}, Coord{0, -2}, Coord{1, -2}};
    }
    if(from == Rotation::Left && to == Rotation::Reverse)
    {
        return {Coord{0, 0}, Coord{-1, 0}, Coord{-1, -1}, Coord{0, 2}, Coord{-1, 2}};
    }
    if(from == Rotation::Left && to == Rotation::Spawn)
    {
        return {Coord{0, 0}, Coord{-1, 0}, Coord{-1, -1}, Coord{0, 2}, Coord{-1, 2}};
    }
    if(from == Rotation::Spawn && to == Rotation::Left)
    {
        return {Coord{0, 0}, Coord{1, 0}, Coord{1, 1}, Coord{0, -2}, Coord{1, -2}};
    }
    return {};
}

} // namespace sgl::tetris
