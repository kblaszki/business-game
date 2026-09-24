#include <arkanoid/app/Bindings.hpp>
#include <arkanoid/app/HudModel.hpp>

#include <arkanoid/sim/State.hpp>

#include <eng/core/Time.hpp>
#include <eng/input/InputEvent.hpp>
#include <eng/input/InputState.hpp>
#include <eng/input/Key.hpp>

#include <gtest/gtest.h>

TEST(HudModelShould, midGameHasNoBanner)
{
    arkanoid::State state{};
    state.score = 40;
    state.lives = 2;

    const arkanoid::HudModel hud = arkanoid::makeHud(state);

    EXPECT_EQ(hud.score, "Score 40");
    EXPECT_EQ(hud.lives, "Lives 2");
    EXPECT_TRUE(hud.effect.empty());
    EXPECT_FALSE(hud.banner.has_value());
    EXPECT_FALSE(hud.hint.has_value());
}

TEST(HudModelShould, stage3ClearedShowsYouWin)
{
    arkanoid::State state{};
    state.cleared = true;
    state.stage = arkanoid::StageId::Stage3;

    const arkanoid::HudModel hud = arkanoid::makeHud(state);

    ASSERT_TRUE(hud.banner.has_value());
    EXPECT_EQ(*hud.banner, "You win");
    ASSERT_TRUE(hud.hint.has_value());
    EXPECT_EQ(*hud.hint, "Enter - retry   Backspace - menu");
}

TEST(HudModelShould, stage1ClearedShowsNoBanner)
{
    arkanoid::State state{};
    state.cleared = true;
    state.stage = arkanoid::StageId::Stage1;

    const arkanoid::HudModel hud = arkanoid::makeHud(state);

    EXPECT_FALSE(hud.banner.has_value());
    EXPECT_FALSE(hud.hint.has_value());
}

TEST(HudModelShould, overShowsYouLose)
{
    arkanoid::State state{};
    state.over = true;

    const arkanoid::HudModel hud = arkanoid::makeHud(state);

    ASSERT_TRUE(hud.banner.has_value());
    EXPECT_EQ(*hud.banner, "You lose");
    ASSERT_TRUE(hud.hint.has_value());
    EXPECT_EQ(*hud.hint, "Enter - retry   Backspace - menu");
}

TEST(HudModelShould, wideEffectStringWhenTimedRemaining)
{
    arkanoid::State state{};
    state.effects.timed = arkanoid::PowerUpKind::Wide;
    state.effects.remaining = eng::Seconds{4.f};

    const arkanoid::HudModel hud = arkanoid::makeHud(state);

    EXPECT_EQ(hud.effect, "Wide");
}

TEST(HudModelShould, defaultBindingsMapsEnterToConfirm)
{
    const arkanoid::Actions actions = arkanoid::makeActions();
    const eng::ActionMap map = arkanoid::defaultBindings(actions);

    eng::InputState input;
    input.apply(eng::KeyDown{.key = eng::Key::Enter}, map);

    EXPECT_TRUE(input.action(actions.confirm).pressed);
}
