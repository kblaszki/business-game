#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <tetris/sim/Types.hpp>

namespace sgl::tetris
{

struct ClearedRows
{
    std::array<int, 4> rows{};
    uint8_t count{};
};

class Grid
{
public:
    static constexpr int kWidth = 10;
    static constexpr int kHeight = 40;
    static constexpr int kVisibleHeight = 20;

    using Row = std::array<Tile, kWidth>;

    [[nodiscard]] bool inside(Coord c) const noexcept;
    [[nodiscard]] bool occupied(Coord c) const noexcept;
    [[nodiscard]] bool fits(std::span<const Coord, 4> cells) const noexcept;
    void lock(std::span<const Coord, 4> cells, PieceType type);
    ClearedRows clearFullRows();
    [[nodiscard]] std::span<const Row, kHeight> rows() const noexcept;

private:
    std::array<Row, kHeight> rows_{};
};

} // namespace sgl::tetris
