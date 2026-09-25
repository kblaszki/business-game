#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <sgl/core/Time.hpp>
#include <sgl/input/InputEvent.hpp>
#include <sgl/input/InputState.hpp>
#include <sgl/input/Key.hpp>
#include <sgl/input/MouseButton.hpp>
#include <sgl/scene/SceneStack.hpp>
#include <tetris/app/Bindings.hpp>
#include <tetris/app/GameOverScene.hpp>
#include <tetris/app/MainMenuScene.hpp>
#include <tetris/app/PlayScene.hpp>
#include <tetris/app/Scenes.hpp>
#include <tetris/sim/Grid.hpp>
#include <tetris/sim/Piece.hpp>
#include <tetris/sim/Types.hpp>
#include <vector>

namespace
{

struct Fixture
{
    sgl::tetris::Actions actions = sgl::tetris::makeActions();
    sgl::FontId font{};
    sgl::tetris::AppServices services{actions, font, 1};
    sgl::ActionMap map = sgl::tetris::defaultBindings(actions);
    sgl::SceneStack stack{sgl::tetris::pause(services)};
};

sgl::InputState inputWith(sgl::Key key, const sgl::ActionMap& map)
{
    sgl::InputState input;
    input.apply(sgl::KeyDown{.key = key}, map);
    return input;
}

sgl::InputState focusLostInput(const sgl::ActionMap& map)
{
    sgl::InputState input;
    input.apply(sgl::FocusLost{}, map);
    return input;
}

sgl::tetris::Grid& mutableGrid(sgl::tetris::TetrisGame& game)
{
    return const_cast<sgl::tetris::Grid&>(game.grid());
}

void lockPadded(sgl::tetris::Grid& grid, std::vector<sgl::tetris::Coord> cells)
{
    int pad = 0;
    auto nextPad = [&]() {
        while(true)
        {
            const sgl::tetris::Coord c{pad % sgl::tetris::Grid::kWidth, 36 + pad / sgl::tetris::Grid::kWidth};
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
        grid.lock(four, sgl::tetris::PieceType::I);
    }
}

void fillCell(sgl::tetris::Grid& grid, sgl::tetris::Coord c)
{
    lockPadded(grid, {c});
}

} // namespace

TEST(ScenesTest, menuStartsPlay)
{
    Fixture f;
    f.stack.push(sgl::tetris::mainMenu(f.services)());

    f.stack.update(inputWith(sgl::Key::Enter, f.map), sgl::kTick);

    auto* play = dynamic_cast<sgl::tetris::PlayScene*>(f.stack.top());
    ASSERT_NE(play, nullptr);
}

TEST(ScenesTest, cancelOnMenuQuits)
{
    Fixture f;
    f.stack.push(sgl::tetris::mainMenu(f.services)());

    f.stack.update(inputWith(sgl::Key::Backspace, f.map), sgl::kTick);

    EXPECT_TRUE(f.stack.quitRequested());
}

TEST(ScenesTest, pauseOnFocusLost)
{
    Fixture f;
    f.stack.push(sgl::tetris::play(f.services)());

    f.stack.update(focusLostInput(f.map), sgl::kTick);

    EXPECT_EQ(f.stack.size(), 2u);
}

TEST(ScenesTest, gameOverPushesResultScene)
{
    Fixture f;
    f.stack.push(sgl::tetris::play(f.services)());

    auto* play = dynamic_cast<sgl::tetris::PlayScene*>(f.stack.top());
    ASSERT_NE(play, nullptr);

    const sgl::tetris::PieceType nextType = play->game().nextQueue()[0];
    sgl::tetris::Grid& grid = mutableGrid(play->game());
    for(sgl::tetris::Coord c: sgl::tetris::cells(sgl::tetris::spawnPiece(nextType)))
    {
        if(!grid.occupied(c))
        {
            fillCell(grid, c);
        }
    }

    f.stack.update(inputWith(sgl::Key::Space, f.map), sgl::kTick);

    auto* over = dynamic_cast<sgl::tetris::GameOverScene*>(f.stack.top());
    ASSERT_NE(over, nullptr);
    EXPECT_EQ(f.stack.size(), 2u);
}

TEST(ScenesTest, retryStartsFreshGame)
{
    Fixture f;
    f.stack.push(sgl::tetris::play(f.services)());

    auto* play = dynamic_cast<sgl::tetris::PlayScene*>(f.stack.top());
    ASSERT_NE(play, nullptr);

    const sgl::tetris::PieceType nextType = play->game().nextQueue()[0];
    sgl::tetris::Grid& grid = mutableGrid(play->game());
    for(sgl::tetris::Coord c: sgl::tetris::cells(sgl::tetris::spawnPiece(nextType)))
    {
        if(!grid.occupied(c))
        {
            fillCell(grid, c);
        }
    }
    f.stack.update(inputWith(sgl::Key::Space, f.map), sgl::kTick);
    ASSERT_NE(dynamic_cast<sgl::tetris::GameOverScene*>(f.stack.top()), nullptr);

    f.stack.update(inputWith(sgl::Key::Enter, f.map), sgl::kTick);

    auto* fresh = dynamic_cast<sgl::tetris::PlayScene*>(f.stack.top());
    ASSERT_NE(fresh, nullptr);
    EXPECT_EQ(f.stack.size(), 1u);
    EXPECT_EQ(fresh->game().score().points, 0u);
    EXPECT_FALSE(fresh->game().over());
}
