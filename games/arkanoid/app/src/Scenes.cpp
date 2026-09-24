#include <arkanoid/app/Scenes.hpp>

#include <arkanoid/app/GameplayScene.hpp>
#include <arkanoid/app/MainMenuScene.hpp>
#include <arkanoid/app/PauseScene.hpp>

#include <memory>

namespace arkanoid
{

eng::SceneFactory mainMenu(const AppServices& services)
{
    return [services]() { return std::make_unique<MainMenuScene>(services); };
}

eng::SceneFactory gameplay(const AppServices& services, StageId stage)
{
    return [services, stage]() { return std::make_unique<GameplayScene>(services, stage); };
}

eng::SceneFactory pause(const AppServices& services)
{
    return [services]() { return std::make_unique<PauseScene>(services); };
}

} // namespace arkanoid
