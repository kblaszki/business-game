/* Created by kblaszki */

#include <controllers/EventController.hpp>
#include <controllers/GameController.hpp>
#include <controllers/ScreenController.hpp>
#include <managers/GameExitManager.hpp>
#include <managers/GameWindowManager.hpp>
#include <managers/KeyboardManager.hpp>
#include <managers/MouseManager.hpp>
#include <screens/MenuScreen.hpp>
#include <window/WindowSFML.hpp>

#include <filesystem>
#include <resources/ResourceManager.hpp>

namespace
{
std::filesystem::path resourcesRootFromArgv0(const char* argv0)
{
    std::error_code ec;
    auto executablePath = std::filesystem::absolute(argv0, ec);
    if(ec)
    {
        executablePath = argv0;
    }
    executablePath = std::filesystem::weakly_canonical(executablePath, ec);
    return executablePath.parent_path() / "resources";
}
} // namespace

int main(int argc, char* argv[])
{
    const char* argv0 = (argc > 0 and argv[0] != nullptr) ? argv[0] : "game";
    ResourceManager resources{resourcesRootFromArgv0(argv0)};

    auto window = std::make_unique<WindowSFML>();
    auto eventController = std::make_unique<EventController>(*window);
    eventController->emplace<GameExitManager>(*window);
    eventController->emplace<GameWindowManager>(*window);
    eventController->emplace<KeyboardManager>();
    eventController->emplace<MouseManager>();

    const auto escapeKeyRegistration = eventController->get<ManagerOf::Keyboard>().registerKeyHandler(
        sf::Keyboard::Key::Escape, [&w = *window](const KeyStatus status, const sf::Event::KeyPressed&) {
            if(KeyStatus::Released == status)
            {
                w.close();
            }
        });

    auto stageController = std::make_unique<ScreenController>(*window, [&](ScreenUpdaterI& screenUpdater) {
        return std::make_unique<MenuScreen>(*eventController, *window, screenUpdater, resources);
    });

    GameController game{std::move(window), std::move(eventController), std::move(stageController)};
    game.run();

    return EXIT_SUCCESS;
}
