#include <algorithm>
#include <ranges>
#include <tetris/sim/Grid.hpp>

namespace sgl::tetris
{

bool Grid::inside(Coord c) const noexcept
{
    return c.x >= 0 && c.x < kWidth && c.y >= 0 && c.y < kHeight;
}

bool Grid::occupied(Coord c) const noexcept
{
    if(!inside(c))
    {
        return true;
    }
    return rows_[static_cast<std::size_t>(c.y)][static_cast<std::size_t>(c.x)].has_value();
}

bool Grid::fits(std::span<const Coord, 4> cells) const noexcept
{
    return std::ranges::all_of(cells, [this](Coord c) { return !occupied(c); });
}

void Grid::lock(std::span<const Coord, 4> cells, PieceType type)
{
    for(Coord c: cells)
    {
        rows_[static_cast<std::size_t>(c.y)][static_cast<std::size_t>(c.x)] = type;
    }
}

ClearedRows Grid::clearFullRows()
{
    ClearedRows cleared{};
    for(int y = 0; y < kHeight; ++y)
    {
        if(std::ranges::all_of(rows_[static_cast<std::size_t>(y)], [](const Tile& tile) { return tile.has_value(); }))
        {
            cleared.rows[cleared.count++] = y;
        }
    }

    const auto isFull = [](const Row& row) {
        return std::ranges::all_of(row, [](const Tile& tile) { return tile.has_value(); });
    };

    const auto [first, last] = std::ranges::remove_if(rows_, isFull);
    std::fill(first, last, Row{});
    return cleared;
}

std::span<const Grid::Row, Grid::kHeight> Grid::rows() const noexcept
{
    return rows_;
}

} // namespace sgl::tetris
