#include <algorithm>
#include <gtest/gtest.h>
#include <optional>
#include <tetris/sim/Grid.hpp>
#include <tetris/sim/Piece.hpp>
#include <tetris/sim/Srs.hpp>
#include <tetris/sim/Types.hpp>
#include <vector>

namespace
{

using sgl::tetris::Cells;
using sgl::tetris::cells;
using sgl::tetris::Coord;
using sgl::tetris::Grid;
using sgl::tetris::kicks;
using sgl::tetris::KickTable;
using sgl::tetris::Piece;
using sgl::tetris::PieceType;
using sgl::tetris::rotate;
using sgl::tetris::Rotation;
using sgl::tetris::tryRotate;
using sgl::tetris::Turn;

void lockPadded(Grid& grid, std::vector<Coord> cellList)
{
    int pad = 0;
    auto nextPad = [&]() {
        while(true)
        {
            const Coord c{pad % Grid::kWidth, 36 + pad / Grid::kWidth};
            ++pad;
            if(!grid.occupied(c) && std::find(cellList.begin(), cellList.end(), c) == cellList.end())
            {
                return c;
            }
        }
    };
    while(cellList.size() % 4 != 0)
    {
        cellList.push_back(nextPad());
    }
    for(std::size_t i = 0; i < cellList.size(); i += 4)
    {
        const Cells four{cellList[i], cellList[i + 1], cellList[i + 2], cellList[i + 3]};
        ASSERT_TRUE(grid.fits(four));
        grid.lock(four, PieceType::I);
    }
}

[[nodiscard]] bool containsCell(const Cells& cells, Coord c)
{
    return std::find(cells.begin(), cells.end(), c) != cells.end();
}

void blockEarlierKicks(Grid& grid, Piece piece, Turn turn, int kickIndex1Based)
{
    const Rotation to = rotate(piece.rotation, turn);
    const KickTable table = kicks(piece.type, piece.rotation, turn);
    const Cells successCells =
        cells(Piece{.type = piece.type,
                    .rotation = to,
                    .origin = piece.origin + table[static_cast<std::size_t>(kickIndex1Based - 1)]});

    std::vector<Coord> blockers;
    for(int i = 0; i < kickIndex1Based - 1; ++i)
    {
        const Cells failed = cells(
            Piece{.type = piece.type, .rotation = to, .origin = piece.origin + table[static_cast<std::size_t>(i)]});
        for(Coord c: failed)
        {
            if(!containsCell(successCells, c) && !grid.occupied(c))
            {
                blockers.push_back(c);
                break;
            }
        }
    }
    lockPadded(grid, blockers);
}

void expectKick(PieceType type, Rotation from, Turn turn, int kickIndex1Based, Coord origin)
{
    Grid grid{};
    const Piece piece{.type = type, .rotation = from, .origin = origin};
    const KickTable table = kicks(type, from, turn);
    const Coord expectedOrigin = origin + table[static_cast<std::size_t>(kickIndex1Based - 1)];

    if(kickIndex1Based > 1)
    {
        blockEarlierKicks(grid, piece, turn, kickIndex1Based);
    }

    const std::optional<Piece> result = tryRotate(grid, piece, turn);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->origin, expectedOrigin);
    EXPECT_EQ(result->rotation, rotate(from, turn));
}

} // namespace

TEST(SrsTest, spawnCellsMatchGuideline)
{
    {
        const Piece p = sgl::tetris::spawnPiece(PieceType::I);
        EXPECT_EQ(p.origin, (Coord{3, 18}));
        EXPECT_EQ(cells(p), (Cells{Coord{3, 20}, Coord{4, 20}, Coord{5, 20}, Coord{6, 20}}));
    }
    {
        const Piece p = sgl::tetris::spawnPiece(PieceType::O);
        EXPECT_EQ(p.origin, (Coord{3, 20}));
        EXPECT_EQ(cells(p), (Cells{Coord{3, 20}, Coord{4, 20}, Coord{3, 21}, Coord{4, 21}}));
    }
    {
        const Piece p = sgl::tetris::spawnPiece(PieceType::T);
        EXPECT_EQ(p.origin, (Coord{3, 19}));
        EXPECT_EQ(cells(p), (Cells{Coord{3, 20}, Coord{4, 20}, Coord{5, 20}, Coord{4, 21}}));
    }
    {
        const Piece p = sgl::tetris::spawnPiece(PieceType::J);
        EXPECT_EQ(cells(p), (Cells{Coord{3, 21}, Coord{3, 20}, Coord{4, 20}, Coord{5, 20}}));
    }
    {
        const Piece p = sgl::tetris::spawnPiece(PieceType::L);
        EXPECT_EQ(cells(p), (Cells{Coord{5, 21}, Coord{3, 20}, Coord{4, 20}, Coord{5, 20}}));
    }
    {
        const Piece p = sgl::tetris::spawnPiece(PieceType::S);
        EXPECT_EQ(cells(p), (Cells{Coord{4, 21}, Coord{5, 21}, Coord{3, 20}, Coord{4, 20}}));
    }
    {
        const Piece p = sgl::tetris::spawnPiece(PieceType::Z);
        EXPECT_EQ(cells(p), (Cells{Coord{3, 21}, Coord{4, 21}, Coord{4, 20}, Coord{5, 20}}));
    }
}

TEST(SrsTest, cwFourTimesIsIdentity)
{
    Grid grid{};
    Piece piece = sgl::tetris::spawnPiece(PieceType::T);
    const Coord origin = piece.origin;
    for(int i = 0; i < 4; ++i)
    {
        const auto next = tryRotate(grid, piece, Turn::Cw);
        ASSERT_TRUE(next.has_value());
        piece = *next;
    }
    EXPECT_EQ(piece.rotation, Rotation::Spawn);
    EXPECT_EQ(piece.origin, origin);
}

TEST(SrsTest, oNeverMoves)
{
    Grid grid{};
    Piece piece = sgl::tetris::spawnPiece(PieceType::O);
    const Cells before = cells(piece);
    const auto cw = tryRotate(grid, piece, Turn::Cw);
    ASSERT_TRUE(cw.has_value());
    EXPECT_EQ(cw->origin, piece.origin);
    EXPECT_EQ(cells(*cw), before);
    const auto ccw = tryRotate(grid, piece, Turn::Ccw);
    ASSERT_TRUE(ccw.has_value());
    EXPECT_EQ(ccw->origin, piece.origin);
    EXPECT_EQ(cells(*ccw), before);
}

TEST(SrsTest, jlstzKickTest1)
{
    expectKick(PieceType::T, Rotation::Spawn, Turn::Cw, 1, {4, 10});
}

TEST(SrsTest, jlstzKickTest2)
{
    expectKick(PieceType::T, Rotation::Spawn, Turn::Cw, 2, {4, 10});
}

TEST(SrsTest, jlstzKickTest3)
{
    expectKick(PieceType::T, Rotation::Spawn, Turn::Cw, 3, {4, 10});
}

TEST(SrsTest, jlstzKickTest4)
{
    expectKick(PieceType::T, Rotation::Spawn, Turn::Cw, 4, {4, 10});
}

TEST(SrsTest, jlstzKickTest5)
{
    expectKick(PieceType::T, Rotation::Spawn, Turn::Cw, 5, {4, 10});
}

TEST(SrsTest, iKickTest1)
{
    expectKick(PieceType::I, Rotation::Spawn, Turn::Cw, 1, {3, 10});
}

TEST(SrsTest, iKickTest2)
{
    expectKick(PieceType::I, Rotation::Spawn, Turn::Cw, 2, {3, 10});
}

TEST(SrsTest, iKickTest3)
{
    expectKick(PieceType::I, Rotation::Spawn, Turn::Cw, 3, {3, 10});
}

TEST(SrsTest, iKickTest4)
{
    expectKick(PieceType::I, Rotation::Spawn, Turn::Cw, 4, {3, 10});
}

TEST(SrsTest, iKickTest5)
{
    expectKick(PieceType::I, Rotation::Spawn, Turn::Cw, 5, {3, 10});
}

TEST(SrsTest, rotationFailsWhenAllKicksBlocked)
{
    Grid grid{};
    const Piece piece{.type = PieceType::T, .rotation = Rotation::Spawn, .origin = {4, 10}};
    const Rotation to = Rotation::Right;
    const KickTable table = kicks(PieceType::T, Rotation::Spawn, Turn::Cw);
    std::vector<Coord> blockers;
    for(Coord kick: table)
    {
        const Cells failed = cells(Piece{.type = PieceType::T, .rotation = to, .origin = piece.origin + kick});
        for(Coord c: failed)
        {
            if(!grid.occupied(c) && std::find(blockers.begin(), blockers.end(), c) == blockers.end())
            {
                blockers.push_back(c);
                break;
            }
        }
    }
    lockPadded(grid, blockers);
    EXPECT_FALSE(tryRotate(grid, piece, Turn::Cw).has_value());
}

TEST(SrsTest, dropToFloorLandsOnStack)
{
    Grid grid{};
    lockPadded(grid, {{3, 0}, {4, 0}, {5, 0}, {6, 0}});
    const Piece piece{.type = PieceType::T, .rotation = Rotation::Spawn, .origin = {3, 10}};
    const Piece dropped = sgl::tetris::dropToFloor(grid, piece);
    EXPECT_EQ(dropped.origin, (Coord{3, 0}));
    EXPECT_TRUE(grid.fits(cells(dropped)));
    EXPECT_FALSE(sgl::tetris::tryShift(grid, dropped, {0, -1}).has_value());
}
