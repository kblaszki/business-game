#include <memory>
#include <tetris/app/GameOverScene.hpp>
#include <tetris/app/MainMenuScene.hpp>
#include <tetris/app/PauseScene.hpp>
#include <tetris/app/PlayScene.hpp>
#include <tetris/app/Scenes.hpp>

namespace sgl::tetris
{

sgl::SceneFactory mainMenu(const AppServices& services)
{
    return [services]() { return std::make_unique<MainMenuScene>(services); };
}

sgl::SceneFactory play(const AppServices& services)
{
    return [services]() { return std::make_unique<PlayScene>(services); };
}

sgl::SceneFactory pause(const AppServices& services)
{
    return [services]() { return std::make_unique<PauseScene>(services); };
}

sgl::SceneFactory gameOver(const AppServices& services, std::uint32_t score)
{
    return [services, score]() { return std::make_unique<GameOverScene>(services, score); };
}

} // namespace sgl::tetris
