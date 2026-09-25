#include <algorithm>
#include <gtest/gtest.h>
#include <tetris/sim/Grid.hpp>
#include <tetris/sim/Piece.hpp>
#include <tetris/sim/Types.hpp>
#include <vector>

namespace
{

using sgl::tetris::Cells;
using sgl::tetris::Coord;
using sgl::tetris::Grid;
using sgl::tetris::PieceType;
using sgl::tetris::Tile;

void lockPadded(Grid& grid, std::vector<Coord> cells)
{
    int pad = 0;
    auto nextPad = [&]() {
        while(true)
        {
            const Coord c{pad % Grid::kWidth, 36 + pad / Grid::kWidth};
            ++pad;
            if(!grid.occupied(c) && std::find(cells.begin(), cells.end(), c) == cells.end())
            {
                return c;
            }
        }
    };
    while(cells.size() % 4 != 0)
    {
        cells.push_back(nextPad());
    }
    for(std::size_t i = 0; i < cells.size(); i += 4)
    {
        const Cells four{cells[i], cells[i + 1], cells[i + 2], cells[i + 3]};
        ASSERT_TRUE(grid.fits(four));
        grid.lock(four, PieceType::I);
    }
}

void fillRow(Grid& grid, int y)
{
    lockPadded(grid, {{0, y}, {1, y}, {2, y}, {3, y}, {4, y}, {5, y}, {6, y}, {7, y}, {8, y}, {9, y}});
}

void fillCell(Grid& grid, Coord c)
{
    lockPadded(grid, {c});
}

} // namespace

TEST(GridTest, emptyGridFitsSpawn)
{
    const Grid grid{};
    const sgl::tetris::Piece piece = sgl::tetris::spawnPiece(PieceType::T);
    const Cells c = sgl::tetris::cells(piece);
    EXPECT_TRUE(grid.fits(c));
}

TEST(GridTest, outsideCountsAsOccupied)
{
    const Grid grid{};
    EXPECT_TRUE(grid.occupied({-1, 0}));
    EXPECT_TRUE(grid.occupied({Grid::kWidth, 0}));
    EXPECT_TRUE(grid.occupied({0, -1}));
    EXPECT_TRUE(grid.occupied({0, Grid::kHeight}));
    EXPECT_FALSE(grid.occupied({0, 0}));
}

TEST(GridTest, lockThenOccupied)
{
    Grid grid{};
    const Cells c{Coord{0, 0}, Coord{1, 0}, Coord{2, 0}, Coord{3, 0}};
    ASSERT_TRUE(grid.fits(c));
    grid.lock(c, PieceType::I);
    EXPECT_TRUE(grid.occupied({0, 0}));
    EXPECT_TRUE(grid.occupied({3, 0}));
    EXPECT_FALSE(grid.occupied({4, 0}));
    EXPECT_EQ(grid.rows()[0][0], Tile{PieceType::I});
}

TEST(GridTest, clearSingleRowShiftsAboveDown)
{
    Grid grid{};
    fillRow(grid, 0);
    fillCell(grid, {5, 1});

    const sgl::tetris::ClearedRows cleared = grid.clearFullRows();
    EXPECT_EQ(cleared.count, 1);
    EXPECT_EQ(cleared.rows[0], 0);
    EXPECT_EQ(grid.rows()[0][5], Tile{PieceType::I});
    EXPECT_FALSE(grid.occupied({5, 1}));
}

TEST(GridTest, clearNonAdjacentRows)
{
    Grid grid{};
    fillRow(grid, 0);
    fillCell(grid, {4, 1});
    fillRow(grid, 2);

    const sgl::tetris::ClearedRows cleared = grid.clearFullRows();
    EXPECT_EQ(cleared.count, 2);
    EXPECT_EQ(cleared.rows[0], 0);
    EXPECT_EQ(cleared.rows[1], 2);
    EXPECT_EQ(grid.rows()[0][4], Tile{PieceType::I});
    EXPECT_FALSE(grid.occupied({4, 1}));
    EXPECT_FALSE(grid.occupied({0, 2}));
}

TEST(GridTest, clearFourRowsReportsIndices)
{
    Grid grid{};
    fillRow(grid, 0);
    fillRow(grid, 1);
    fillRow(grid, 2);
    fillRow(grid, 3);

    const sgl::tetris::ClearedRows cleared = grid.clearFullRows();
    EXPECT_EQ(cleared.count, 4);
    EXPECT_EQ(cleared.rows[0], 0);
    EXPECT_EQ(cleared.rows[1], 1);
    EXPECT_EQ(cleared.rows[2], 2);
    EXPECT_EQ(cleared.rows[3], 3);
    for(int y = 0; y < 4; ++y)
    {
        for(int x = 0; x < Grid::kWidth; ++x)
        {
            EXPECT_FALSE(grid.occupied({x, y}));
        }
    }
}
