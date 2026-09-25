#include <arkanoid/app/Bindings.hpp>
#include <arkanoid/app/HudModel.hpp>
#include <arkanoid/sim/State.hpp>
#include <gtest/gtest.h>
#include <sgl/core/Time.hpp>
#include <sgl/input/InputEvent.hpp>
#include <sgl/input/InputState.hpp>
#include <sgl/input/Key.hpp>

TEST(HudModelShould, midGameHasNoBanner)
{
    sgl::arkanoid::State state{};
    state.score = 40;
    state.lives = 2;

    const sgl::arkanoid::HudModel hud = sgl::arkanoid::makeHud(state);

    EXPECT_EQ(hud.score, "Score 40");
    EXPECT_EQ(hud.lives, "Lives 2");
    EXPECT_TRUE(hud.effect.empty());
    EXPECT_FALSE(hud.banner.has_value());
    EXPECT_FALSE(hud.hint.has_value());
}

TEST(HudModelShould, stage3ClearedShowsYouWin)
{
    sgl::arkanoid::State state{};
    state.cleared = true;
    state.stage = sgl::arkanoid::StageId::Stage3;

    const sgl::arkanoid::HudModel hud = sgl::arkanoid::makeHud(state);

    ASSERT_TRUE(hud.banner.has_value());
    EXPECT_EQ(*hud.banner, "You win");
    ASSERT_TRUE(hud.hint.has_value());
    EXPECT_EQ(*hud.hint, "Enter - retry   Backspace - menu");
}

TEST(HudModelShould, stage1ClearedShowsNoBanner)
{
    sgl::arkanoid::State state{};
    state.cleared = true;
    state.stage = sgl::arkanoid::StageId::Stage1;

    const sgl::arkanoid::HudModel hud = sgl::arkanoid::makeHud(state);

    EXPECT_FALSE(hud.banner.has_value());
    EXPECT_FALSE(hud.hint.has_value());
}

TEST(HudModelShould, overShowsYouLose)
{
    sgl::arkanoid::State state{};
    state.over = true;

    const sgl::arkanoid::HudModel hud = sgl::arkanoid::makeHud(state);

    ASSERT_TRUE(hud.banner.has_value());
    EXPECT_EQ(*hud.banner, "You lose");
    ASSERT_TRUE(hud.hint.has_value());
    EXPECT_EQ(*hud.hint, "Enter - retry   Backspace - menu");
}

TEST(HudModelShould, wideEffectStringWhenTimedRemaining)
{
    sgl::arkanoid::State state{};
    state.effects.timed = sgl::arkanoid::PowerUpKind::Wide;
    state.effects.remaining = sgl::Seconds{4.f};

    const sgl::arkanoid::HudModel hud = sgl::arkanoid::makeHud(state);

    EXPECT_EQ(hud.effect, "Wide");
}

TEST(HudModelShould, defaultBindingsMapsEnterToConfirm)
{
    const sgl::arkanoid::Actions actions = sgl::arkanoid::makeActions();
    const sgl::ActionMap map = sgl::arkanoid::defaultBindings(actions);

    sgl::InputState input;
    input.apply(sgl::KeyDown{.key = sgl::Key::Enter}, map);

    EXPECT_TRUE(input.action(actions.confirm).pressed);
}
