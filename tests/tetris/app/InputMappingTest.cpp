#include <gtest/gtest.h>
#include <sgl/input/InputEvent.hpp>
#include <sgl/input/InputState.hpp>
#include <sgl/input/Key.hpp>
#include <tetris/app/Bindings.hpp>
#include <tetris/app/InputMapping.hpp>

namespace
{

struct Fixture
{
    sgl::tetris::Actions actions = sgl::tetris::makeActions();
    sgl::ActionMap map = sgl::tetris::defaultBindings(actions);
};

} // namespace

TEST(InputMappingTest, mapsEveryAction)
{
    Fixture f;
    sgl::InputState input;
    input.apply(sgl::KeyDown{.key = sgl::Key::Left}, f.map);
    input.apply(sgl::KeyDown{.key = sgl::Key::Right}, f.map);
    input.apply(sgl::KeyDown{.key = sgl::Key::Down}, f.map);
    input.apply(sgl::KeyDown{.key = sgl::Key::Space}, f.map);
    input.apply(sgl::KeyDown{.key = sgl::Key::Up}, f.map);
    input.apply(sgl::KeyDown{.key = sgl::Key::Z}, f.map);
    input.apply(sgl::KeyDown{.key = sgl::Key::C}, f.map);

    const sgl::tetris::TetrisInput mapped = sgl::tetris::toTetrisInput(input, f.actions);
    EXPECT_TRUE(mapped.leftPressed);
    EXPECT_TRUE(mapped.leftHeld);
    EXPECT_TRUE(mapped.rightPressed);
    EXPECT_TRUE(mapped.rightHeld);
    EXPECT_TRUE(mapped.softDropHeld);
    EXPECT_TRUE(mapped.hardDrop);
    EXPECT_TRUE(mapped.rotateCw);
    EXPECT_TRUE(mapped.rotateCcw);
    EXPECT_TRUE(mapped.hold);

    sgl::InputState xOnly;
    xOnly.apply(sgl::KeyDown{.key = sgl::Key::X}, f.map);
    EXPECT_TRUE(sgl::tetris::toTetrisInput(xOnly, f.actions).rotateCw);

    sgl::InputState shiftOnly;
    shiftOnly.apply(sgl::KeyDown{.key = sgl::Key::LShift}, f.map);
    EXPECT_TRUE(sgl::tetris::toTetrisInput(shiftOnly, f.actions).hold);
}

TEST(InputMappingTest, tapWithinFrameProducesPressed)
{
    Fixture f;
    sgl::InputState input;
    input.apply(sgl::KeyDown{.key = sgl::Key::Space}, f.map);
    input.apply(sgl::KeyUp{.key = sgl::Key::Space}, f.map);

    const sgl::tetris::TetrisInput mapped = sgl::tetris::toTetrisInput(input, f.actions);
    EXPECT_TRUE(mapped.hardDrop);
    EXPECT_FALSE(input.action(f.actions.hardDrop).held);
}
