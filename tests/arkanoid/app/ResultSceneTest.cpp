#include <arkanoid/app/Bindings.hpp>
#include <arkanoid/app/GameplayScene.hpp>
#include <arkanoid/app/ResultScene.hpp>
#include <arkanoid/app/Scenes.hpp>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <sgl/core/Time.hpp>
#include <sgl/input/InputEvent.hpp>
#include <sgl/input/InputState.hpp>
#include <sgl/input/Key.hpp>
#include <sgl/render/DrawCommand.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <sgl/save/HighScoreTable.hpp>
#include <sgl/scene/SceneStack.hpp>
#include <string>
#include <system_error>
#include <variant>

namespace
{

[[nodiscard]] std::filesystem::path uniqueTempPath(std::string_view suffix)
{
    const auto stamp = std::to_string(std::chrono::steady_clock::now().time_since_epoch().count());
    return std::filesystem::temp_directory_path() / ("arkanoid_result_" + stamp + std::string{suffix});
}

[[nodiscard]] bool queueContainsText(sgl::RenderQueue& queue, std::string_view needle)
{
    for(const sgl::Entry& entry: queue.sorted())
    {
        if(const auto* text = std::get_if<sgl::TextCmd>(&entry.command))
        {
            if(text->text.find(needle) != std::string::npos)
            {
                return true;
            }
        }
    }
    return false;
}

void clearAllBricks(sgl::arkanoid::State& state)
{
    for(auto& brick: state.bricks)
    {
        brick.alive = false;
    }
    state.cleared = false;
}

} // namespace

TEST(ResultSceneTest, newRecordShownForTopScore)
{
    sgl::arkanoid::Actions actions = sgl::arkanoid::makeActions();
    sgl::arkanoid::TextureIds textures{};
    sgl::FontId font{};
    const std::filesystem::path scoresPath = uniqueTempPath(".scores");
    sgl::HighScoreTable highScores{sgl::arkanoid::kHighScoreCapacity};
    const sgl::arkanoid::AppServices services{actions, textures, font, highScores, scoresPath};

    sgl::arkanoid::ResultScene scene{services, sgl::arkanoid::Outcome::Won, 999u, 0u, false};
    sgl::RenderQueue queue;
    scene.render(queue);

    EXPECT_TRUE(queueContainsText(queue, "New record"));
    EXPECT_FALSE(queueContainsText(queue, "Could not save high score"));

    std::error_code ec;
    std::filesystem::remove(scoresPath, ec);
}

TEST(ResultSceneTest, saveFailureShownNotFatal)
{
    sgl::arkanoid::Actions actions = sgl::arkanoid::makeActions();
    sgl::arkanoid::TextureIds textures{};
    sgl::FontId font{};

    const std::filesystem::path blocker = uniqueTempPath(".blocker");
    {
        std::ofstream out(blocker);
        out << "not-a-directory";
    }
    const std::filesystem::path scoresPath = blocker / "arkanoid.scores";

    sgl::HighScoreTable highScores{sgl::arkanoid::kHighScoreCapacity};
    const sgl::arkanoid::AppServices services{actions, textures, font, highScores, scoresPath};
    sgl::ActionMap map = sgl::arkanoid::defaultBindings(actions);
    sgl::SceneStack stack{sgl::arkanoid::pause(services)};
    stack.push(sgl::arkanoid::gameplay(services, sgl::arkanoid::StageId::Stage3)());

    auto* play = dynamic_cast<sgl::arkanoid::GameplayScene*>(stack.top());
    ASSERT_NE(play, nullptr);
    play->state().score = 100;
    clearAllBricks(play->state());

    stack.update(sgl::InputState{}, sgl::kTick);

    auto* result = dynamic_cast<sgl::arkanoid::ResultScene*>(stack.top());
    ASSERT_NE(result, nullptr);
    EXPECT_TRUE(result->saveFailed());
    EXPECT_TRUE(result->rank().has_value());

    sgl::RenderQueue queue;
    result->render(queue);
    EXPECT_TRUE(queueContainsText(queue, "Could not save high score"));
    EXPECT_TRUE(queueContainsText(queue, "New record"));

    sgl::InputState confirm;
    confirm.apply(sgl::KeyDown{.key = sgl::Key::Enter}, map);
    stack.update(confirm, sgl::kTick);

    auto* fresh = dynamic_cast<sgl::arkanoid::GameplayScene*>(stack.top());
    ASSERT_NE(fresh, nullptr);
    EXPECT_EQ(fresh->state().stage, sgl::arkanoid::StageId::Stage1);

    std::error_code ec;
    std::filesystem::remove(blocker, ec);
}
