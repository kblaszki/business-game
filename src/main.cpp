/* Created by kblaszki */

#include <controllers/EventController.hpp>
#include <controllers/GameController.hpp>
#include <controllers/ScreenController.hpp>
#include <managers/GameExitManager.hpp>
#include <managers/KeyboardManager.hpp>
#include <managers/MouseManager.hpp>
#include <window/WindowSFML.hpp>

int main()
{
    auto window = std::make_unique<WindowSFML>();
    auto eventController = std::make_unique<EventController>(*window);
    eventController->emplace<GameExitManager>(*window);
    eventController->emplace<KeyboardManager>();
    eventController->emplace<MouseManager>();

    eventController->get<ManagerOf::Keyboard>().registerKeyHandler(
        sf::Keyboard::Escape, [&w = *window](const KeyStatus status, const sf::Event::KeyEvent&) {
            if(KeyStatus::Released == status)
            {
                w.close();
            }
        });

    auto stageController = std::make_unique<ScreenController>(*eventController, *window);

    GameController game{std::move(window), std::move(eventController), std::move(stageController)};
    game.run();

    return EXIT_SUCCESS;
}