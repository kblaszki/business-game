#include <arkanoid/app/Bindings.hpp>
#include <arkanoid/app/GameplayScene.hpp>
#include <arkanoid/app/MainMenuScene.hpp>
#include <arkanoid/app/Scenes.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <sgl/core/Time.hpp>
#include <sgl/input/InputEvent.hpp>
#include <sgl/input/InputState.hpp>
#include <sgl/input/Key.hpp>
#include <sgl/scene/SceneStack.hpp>

namespace
{

struct Fixture
{
    sgl::arkanoid::Actions actions = sgl::arkanoid::makeActions();
    sgl::arkanoid::TextureIds textures{};
    sgl::FontId font{};
    sgl::arkanoid::AppServices services{actions, textures, font};
    sgl::ActionMap map = sgl::arkanoid::defaultBindings(actions);
    sgl::SceneStack stack{sgl::arkanoid::pause(services)};
};

sgl::InputState inputWith(sgl::Key key, const sgl::ActionMap& map)
{
    sgl::InputState input;
    input.apply(sgl::KeyDown{.key = key}, map);
    return input;
}

} // namespace

TEST(ScenesShould, enterOnMainMenuStartsStage1)
{
    Fixture f;
    f.stack.push(sgl::arkanoid::mainMenu(f.services)());

    f.stack.update(inputWith(sgl::Key::Enter, f.map), sgl::kTick);

    auto* play = dynamic_cast<sgl::arkanoid::GameplayScene*>(f.stack.top());
    ASSERT_NE(play, nullptr);
    EXPECT_EQ(play->state().stage, sgl::arkanoid::StageId::Stage1);
}

TEST(ScenesShould, backspaceOnMainMenuQuits)
{
    Fixture f;
    f.stack.push(sgl::arkanoid::mainMenu(f.services)());

    f.stack.update(inputWith(sgl::Key::Backspace, f.map), sgl::kTick);

    EXPECT_TRUE(f.stack.quitRequested());
}

TEST(ScenesShould, clearingStage1AdvancesToStage2)
{
    Fixture f;
    f.stack.push(sgl::arkanoid::gameplay(f.services, sgl::arkanoid::StageId::Stage1)());

    auto* play = dynamic_cast<sgl::arkanoid::GameplayScene*>(f.stack.top());
    ASSERT_NE(play, nullptr);

    const std::uint32_t score = play->state().score;
    for(auto& brick: play->state().bricks)
    {
        brick.alive = false;
    }
    play->state().cleared = false;

    f.stack.update(sgl::InputState{}, sgl::kTick);

    EXPECT_EQ(play->state().stage, sgl::arkanoid::StageId::Stage2);
    EXPECT_EQ(play->state().score, score);
}

TEST(ScenesShould, clearingStage3ShowsYouWinBanner)
{
    Fixture f;
    f.stack.push(sgl::arkanoid::gameplay(f.services, sgl::arkanoid::StageId::Stage3)());

    auto* play = dynamic_cast<sgl::arkanoid::GameplayScene*>(f.stack.top());
    ASSERT_NE(play, nullptr);

    for(auto& brick: play->state().bricks)
    {
        brick.alive = false;
    }
    play->state().cleared = false;

    f.stack.update(sgl::InputState{}, sgl::kTick);

    EXPECT_EQ(play->state().stage, sgl::arkanoid::StageId::Stage3);
    ASSERT_TRUE(play->hud().banner.has_value());
    EXPECT_EQ(*play->hud().banner, "You win");
}

TEST(ScenesShould, escapeWhilePlayingPushesPause)
{
    Fixture f;
    f.stack.push(sgl::arkanoid::gameplay(f.services, sgl::arkanoid::StageId::Stage1)());

    f.stack.update(inputWith(sgl::Key::Escape, f.map), sgl::kTick);

    EXPECT_EQ(f.stack.size(), 2u);
}
