#include <arkanoid/app/GameplayScene.hpp>
#include <arkanoid/app/MainMenuScene.hpp>
#include <arkanoid/app/PauseScene.hpp>
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

} // namespace sgl::arkanoid
