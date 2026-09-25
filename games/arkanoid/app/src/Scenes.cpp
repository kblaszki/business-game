#include <arkanoid/app/GameplayScene.hpp>
#include <arkanoid/app/MainMenuScene.hpp>
#include <arkanoid/app/PauseScene.hpp>
#include <arkanoid/app/ResultScene.hpp>
#include <arkanoid/app/Scenes.hpp>
#include <memory>

namespace sgl::arkanoid
{

sgl::SceneFactory mainMenu(const AppServices& services)
{
    return [services]() { return std::make_unique<MainMenuScene>(services); };
}

sgl::SceneFactory gameplay(const AppServices& services, StageId stage)
{
    return [services, stage]() { return std::make_unique<GameplayScene>(services, stage); };
}

sgl::SceneFactory pause(const AppServices& services)
{
    return [services]() { return std::make_unique<PauseScene>(services); };
}

sgl::SceneFactory result(
    const AppServices& services, Outcome outcome, std::uint32_t score, std::optional<std::size_t> rank, bool saveFailed)
{
    return [services, outcome, score, rank, saveFailed]() {
        return std::make_unique<ResultScene>(services, outcome, score, rank, saveFailed);
    };
}

} // namespace sgl::arkanoid
