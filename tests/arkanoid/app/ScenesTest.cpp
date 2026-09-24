#include <arkanoid/app/Bindings.hpp>
#include <arkanoid/app/GameplayScene.hpp>
#include <arkanoid/app/MainMenuScene.hpp>
#include <arkanoid/app/Scenes.hpp>

#include <eng/core/Time.hpp>
#include <eng/input/InputEvent.hpp>
#include <eng/input/InputState.hpp>
#include <eng/input/Key.hpp>
#include <eng/scene/SceneStack.hpp>

#include <gtest/gtest.h>

#include <memory>

namespace
{

struct Fixture
{
    arkanoid::Actions actions = arkanoid::makeActions();
    arkanoid::TextureIds textures{};
    eng::FontId font{};
    arkanoid::AppServices services{actions, textures, font};
    eng::ActionMap map = arkanoid::defaultBindings(actions);
    eng::SceneStack stack{arkanoid::pause(services)};
};

eng::InputState inputWith(eng::Key key, const eng::ActionMap& map)
{
    eng::InputState input;
    input.apply(eng::KeyDown{.key = key}, map);
    return input;
}

} // namespace

TEST(ScenesShould, enterOnMainMenuStartsStage1)
{
    Fixture f;
    f.stack.push(arkanoid::mainMenu(f.services)());

    f.stack.update(inputWith(eng::Key::Enter, f.map), eng::kTick);

    auto* play = dynamic_cast<arkanoid::GameplayScene*>(f.stack.top());
    ASSERT_NE(play, nullptr);
    EXPECT_EQ(play->state().stage, arkanoid::StageId::Stage1);
}

TEST(ScenesShould, backspaceOnMainMenuQuits)
{
    Fixture f;
    f.stack.push(arkanoid::mainMenu(f.services)());

    f.stack.update(inputWith(eng::Key::Backspace, f.map), eng::kTick);

    EXPECT_TRUE(f.stack.quitRequested());
}

TEST(ScenesShould, clearingStage1AdvancesToStage2)
{
    Fixture f;
    f.stack.push(arkanoid::gameplay(f.services, arkanoid::StageId::Stage1)());

    auto* play = dynamic_cast<arkanoid::GameplayScene*>(f.stack.top());
    ASSERT_NE(play, nullptr);

    const std::uint32_t score = play->state().score;
    for(auto& brick: play->state().bricks)
    {
        brick.alive = false;
    }
    play->state().cleared = false;

    f.stack.update(eng::InputState{}, eng::kTick);

    EXPECT_EQ(play->state().stage, arkanoid::StageId::Stage2);
    EXPECT_EQ(play->state().score, score);
}

TEST(ScenesShould, clearingStage3ShowsYouWinBanner)
{
    Fixture f;
    f.stack.push(arkanoid::gameplay(f.services, arkanoid::StageId::Stage3)());

    auto* play = dynamic_cast<arkanoid::GameplayScene*>(f.stack.top());
    ASSERT_NE(play, nullptr);

    for(auto& brick: play->state().bricks)
    {
        brick.alive = false;
    }
    play->state().cleared = false;

    f.stack.update(eng::InputState{}, eng::kTick);

    EXPECT_EQ(play->state().stage, arkanoid::StageId::Stage3);
    ASSERT_TRUE(play->hud().banner.has_value());
    EXPECT_EQ(*play->hud().banner, "You win");
}

TEST(ScenesShould, escapeWhilePlayingPushesPause)
{
    Fixture f;
    f.stack.push(arkanoid::gameplay(f.services, arkanoid::StageId::Stage1)());

    f.stack.update(inputWith(eng::Key::Escape, f.map), eng::kTick);

    EXPECT_EQ(f.stack.size(), 2u);
}
