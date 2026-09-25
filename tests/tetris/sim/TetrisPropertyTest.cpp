#include <gtest/gtest.h>
#include <sgl/core/Random.hpp>
#include <sgl/core/Time.hpp>
#include <tetris/sim/Events.hpp>
#include <tetris/sim/Grid.hpp>
#include <tetris/sim/Piece.hpp>
#include <tetris/sim/TetrisGame.hpp>

namespace
{

using sgl::Seconds;
using sgl::tetris::Grid;
using sgl::tetris::LinesCleared;
using sgl::tetris::TetrisGame;
using sgl::tetris::TetrisInput;

TetrisInput randomInput(sgl::Pcg32& rng)
{
    auto bit = [&]() { return rng.uniformBelow(2) == 1; };
    TetrisInput in{};
    in.leftPressed = bit();
    in.leftHeld = bit();
    in.rightPressed = bit();
    in.rightHeld = bit();
    in.softDropHeld = bit();
    in.hardDrop = bit();
    in.rotateCw = bit();
    in.rotateCcw = bit();
    in.hold = bit();
    return in;
}

void assertNoCellAbove39(const Grid& grid)
{
    for(int y = 0; y < Grid::kHeight; ++y)
    {
        for(int x = 0; x < Grid::kWidth; ++x)
        {
            if(grid.occupied({x, y}))
            {
                EXPECT_LE(y, 39);
            }
        }
    }
}

} // namespace

TEST(TetrisPropertyTest, invariantsOverRandomInput)
{
    for(std::uint64_t seed = 0; seed < 20; ++seed)
    {
        TetrisGame game{seed};
        sgl::Pcg32 rng{seed + 1000};
        std::uint32_t clearedSum = 0;
        std::uint32_t lastPoints = 0;

        for(int tick = 0; tick < 20000; ++tick)
        {
            if(game.over())
            {
                break;
            }

            if(game.active().has_value())
            {
                const auto c = sgl::tetris::cells(*game.active());
                EXPECT_TRUE(game.grid().fits(c)) << "seed " << seed << " tick " << tick;
                for(const auto& cell: c)
                {
                    EXPECT_LE(cell.y, 39) << "seed " << seed << " tick " << tick;
                }
            }
            assertNoCellAbove39(game.grid());

            const auto events = game.step(randomInput(rng), Seconds{1.f / 60.f});
            for(const auto& e: events)
            {
                if(const auto* lines = std::get_if<LinesCleared>(&e))
                {
                    clearedSum += lines->count;
                }
            }

            EXPECT_GE(game.score().points, lastPoints) << "seed " << seed << " tick " << tick;
            lastPoints = game.score().points;
        }

        EXPECT_EQ(game.score().lines, clearedSum) << "seed " << seed;
    }
}
