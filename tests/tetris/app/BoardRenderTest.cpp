#include <algorithm>
#include <gtest/gtest.h>
#include <sgl/render/DrawCommand.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <tetris/app/BoardRender.hpp>
#include <tetris/sim/Grid.hpp>
#include <tetris/sim/Piece.hpp>
#include <tetris/sim/TetrisGame.hpp>
#include <tetris/sim/Types.hpp>
#include <variant>
#include <vector>

namespace
{

using sgl::tetris::Coord;
using sgl::tetris::Grid;
using sgl::tetris::PieceType;
using sgl::tetris::TetrisGame;

Grid& mutableGrid(TetrisGame& game)
{
    return const_cast<Grid&>(game.grid());
}

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
        const sgl::tetris::Cells four{cells[i], cells[i + 1], cells[i + 2], cells[i + 3]};
        ASSERT_TRUE(grid.fits(four));
        grid.lock(four, PieceType::O);
    }
}

[[nodiscard]] std::size_t countRects(const sgl::RenderQueue& queue)
{
    std::size_t n = 0;
    for(const sgl::Entry& entry: const_cast<sgl::RenderQueue&>(queue).sorted())
    {
        if(std::holds_alternative<sgl::RectCmd>(entry.command))
        {
            ++n;
        }
    }
    return n;
}

[[nodiscard]] bool hasRectAt(const sgl::RenderQueue& queue, sgl::Vec2f pos)
{
    for(const sgl::Entry& entry: const_cast<sgl::RenderQueue&>(queue).sorted())
    {
        if(const auto* rect = std::get_if<sgl::RectCmd>(&entry.command))
        {
            if(rect->rect.pos.x == pos.x && rect->rect.pos.y == pos.y)
            {
                return true;
            }
        }
    }
    return false;
}

[[nodiscard]] std::size_t countGhostRects(const sgl::RenderQueue& queue)
{
    std::size_t n = 0;
    for(const sgl::Entry& entry: const_cast<sgl::RenderQueue&>(queue).sorted())
    {
        if(const auto* rect = std::get_if<sgl::RectCmd>(&entry.command))
        {
            if(rect->fill.a == sgl::tetris::ghostAlpha)
            {
                ++n;
            }
        }
    }
    return n;
}

} // namespace

TEST(BoardRenderTest, oneRectPerLockedCell)
{
    TetrisGame game{1};
    lockPadded(mutableGrid(game), {{0, 0}, {1, 0}, {2, 0}, {3, 0}});

    sgl::RenderQueue empty;
    sgl::tetris::renderBoard(TetrisGame{1}, sgl::tetris::defaultLayout(), empty);
    const std::size_t base = countRects(empty);

    sgl::RenderQueue queue;
    sgl::tetris::renderBoard(game, sgl::tetris::defaultLayout(), queue);
    EXPECT_EQ(countRects(queue), base + 4u);
}

TEST(BoardRenderTest, ghostDrawnBelowActive)
{
    TetrisGame game{1};
    ASSERT_TRUE(game.active().has_value());
    ASSERT_TRUE(game.ghost().has_value());

    sgl::RenderQueue queue;
    sgl::tetris::renderBoard(game, sgl::tetris::defaultLayout(), queue);

    EXPECT_EQ(countGhostRects(queue), 4u);

    float maxGhostDepth = -1.f;
    float minActiveDepth = 1.e9f;
    for(const sgl::Entry& entry: queue.sorted())
    {
        if(const auto* rect = std::get_if<sgl::RectCmd>(&entry.command))
        {
            if(entry.key.layer != sgl::Layer::Actors)
            {
                continue;
            }
            if(rect->fill.a == sgl::tetris::ghostAlpha)
            {
                maxGhostDepth = std::max(maxGhostDepth, entry.key.depth);
            }
            else if(rect->fill.a == 255)
            {
                minActiveDepth = std::min(minActiveDepth, entry.key.depth);
            }
        }
    }
    EXPECT_LT(maxGhostDepth, minActiveDepth);
}

TEST(BoardRenderTest, onlyVisibleRowsDrawn)
{
    TetrisGame game{1};
    lockPadded(mutableGrid(game), {{5, 25}});

    const sgl::tetris::Layout layout = sgl::tetris::defaultLayout();
    sgl::RenderQueue queue;
    sgl::tetris::renderBoard(game, layout, queue);

    const sgl::Vec2f hiddenPos{layout.wellOrigin.x + 5.f * layout.cell,
                               layout.wellOrigin.y + static_cast<float>(Grid::kVisibleHeight - 1 - 25) * layout.cell};
    EXPECT_FALSE(hasRectAt(queue, hiddenPos));
}

TEST(BoardRenderTest, nextAndHoldPanels)
{
    TetrisGame game{7};
    sgl::tetris::TetrisInput hold{};
    hold.hold = true;
    game.step(hold, sgl::Seconds{0.f});
    ASSERT_TRUE(game.held().has_value());

    sgl::RenderQueue beforeHold;
    sgl::tetris::renderBoard(TetrisGame{7}, sgl::tetris::defaultLayout(), beforeHold);
    const std::size_t base = countRects(beforeHold);

    sgl::RenderQueue queue;
    sgl::tetris::renderBoard(game, sgl::tetris::defaultLayout(), queue);
    // hold adds 4 preview cells; next queue always draws 5 pieces (20 cells) in both
    EXPECT_GE(countRects(queue), base + 4u);
    EXPECT_GE(countRects(queue), 4u + 20u);
}
