#include <eng/core/Vec2.hpp>
#include <eng/input/ActionId.hpp>
#include <eng/input/ActionMap.hpp>
#include <eng/input/ActionState.hpp>
#include <eng/input/InputEvent.hpp>
#include <eng/input/InputState.hpp>
#include <eng/input/Key.hpp>
#include <eng/input/MouseButton.hpp>
#include <gtest/gtest.h>

namespace
{

constexpr eng::ActionId kJump{.id = 1};
constexpr eng::ActionId kFire{.id = 2};
constexpr eng::AxisId kMoveX{.id = 10};

} // namespace

TEST(InputStateTest, EdgeLastsOneFrame)
{
    eng::ActionMap map;
    map.bind(eng::Key::Space, kJump);

    eng::InputState state;
    state.apply(eng::KeyDown{.key = eng::Key::Space}, map);

    const eng::ActionState pressed = state.action(kJump);
    EXPECT_TRUE(pressed.pressed);
    EXPECT_TRUE(pressed.held);
    EXPECT_FALSE(pressed.released);

    state.beginFrame();
    const eng::ActionState heldOnly = state.action(kJump);
    EXPECT_FALSE(heldOnly.pressed);
    EXPECT_TRUE(heldOnly.held);
    EXPECT_FALSE(heldOnly.released);
}

TEST(InputStateTest, HoldPersistsAcrossBeginFrame)
{
    eng::ActionMap map;
    map.bind(eng::Key::A, kFire);

    eng::InputState state;
    state.apply(eng::KeyDown{.key = eng::Key::A}, map);
    state.beginFrame();
    state.beginFrame();

    EXPECT_TRUE(state.action(kFire).held);
    EXPECT_FALSE(state.action(kFire).pressed);
    EXPECT_FALSE(state.action(kFire).released);
}

TEST(InputStateTest, BothAxisKeysYieldZero)
{
    eng::ActionMap map;
    map.bindAxis(eng::Key::Left, eng::Key::Right, kMoveX);

    eng::InputState state;
    state.apply(eng::KeyDown{.key = eng::Key::Left}, map);
    state.apply(eng::KeyDown{.key = eng::Key::Right}, map);

    EXPECT_FLOAT_EQ(state.axis(kMoveX), 0.f);
}

TEST(InputStateTest, OneAxisKeyYieldsSignedValue)
{
    eng::ActionMap map;
    map.bindAxis(eng::Key::Left, eng::Key::Right, kMoveX);

    eng::InputState state;
    state.apply(eng::KeyDown{.key = eng::Key::Left}, map);
    EXPECT_FLOAT_EQ(state.axis(kMoveX), -1.f);

    state.apply(eng::KeyUp{.key = eng::Key::Left}, map);
    state.apply(eng::KeyDown{.key = eng::Key::Right}, map);
    EXPECT_FLOAT_EQ(state.axis(kMoveX), 1.f);
}

TEST(InputStateTest, FocusLostClearsHold)
{
    eng::ActionMap map;
    map.bind(eng::Key::D, kFire);
    map.bindAxis(eng::Key::Left, eng::Key::Right, kMoveX);

    eng::InputState state;
    state.apply(eng::KeyDown{.key = eng::Key::D}, map);
    state.apply(eng::KeyDown{.key = eng::Key::Right}, map);
    EXPECT_TRUE(state.action(kFire).held);
    EXPECT_FLOAT_EQ(state.axis(kMoveX), 1.f);

    state.apply(eng::FocusLost{}, map);
    EXPECT_TRUE(state.focusLost());
    EXPECT_FALSE(state.action(kFire).held);
    EXPECT_FLOAT_EQ(state.axis(kMoveX), 0.f);
}

TEST(InputStateTest, UnboundKeyIsNoOp)
{
    eng::ActionMap map;
    map.bind(eng::Key::A, kJump);

    eng::InputState state;
    state.apply(eng::KeyDown{.key = eng::Key::Escape}, map);

    EXPECT_FALSE(state.action(kJump).pressed);
    EXPECT_FALSE(state.action(kJump).held);
    EXPECT_FALSE(state.closeRequested());
    EXPECT_FALSE(state.focusLost());
}

TEST(InputStateTest, MouseDownSetsPointer)
{
    eng::ActionMap map;
    eng::InputState state;

    state.apply(eng::MouseDown{.button = eng::MouseButton::Left, .pos = eng::Vec2f{.x = 12.f, .y = 34.f}}, map);

    ASSERT_TRUE(state.pointer().has_value());
    EXPECT_FLOAT_EQ(state.pointer()->x, 12.f);
    EXPECT_FLOAT_EQ(state.pointer()->y, 34.f);
}

TEST(InputStateTest, CloseRequestedOnWindowClosed)
{
    eng::ActionMap map;
    eng::InputState state;

    state.apply(eng::WindowClosed{}, map);
    EXPECT_TRUE(state.closeRequested());

    state.beginFrame();
    EXPECT_FALSE(state.closeRequested());
}

TEST(ActionMapTest, OneKeyMapsToSeveralActions)
{
    eng::ActionMap map;
    map.bind(eng::Key::Enter, kJump);
    map.bind(eng::Key::Enter, kFire);

    const auto actions = map.actionsFor(eng::Key::Enter);
    ASSERT_EQ(actions.size(), 2u);
    EXPECT_EQ(actions[0], kJump);
    EXPECT_EQ(actions[1], kFire);
}
