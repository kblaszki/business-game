/* Created by kblaszki */

#include <controllers/EventController.hpp>
#include <controllers/GameController.hpp>
#include <controllers/ScreenController.hpp>
#include <managers/ClosedEventManager.hpp>
#include <managers/KeyPressedManager.hpp>
#include <managers/KeyReleasedManager.hpp>
#include <managers/MouseMovedManager.hpp>
#include <window/WindowSFML.hpp>

int main()
{
    auto window = std::make_unique<WindowSFML>();
    auto eventController = std::make_unique<EventController>(*window);
    eventController->emplace<ClosedEventManager>();
    eventController->emplace<KeyPressedManager>();
    eventController->emplace<KeyReleasedManager>();
    eventController->emplace<MouseMovedManager>();

    eventController->get<sf::Event::Closed>().registerHandler([&window]() { window->close(); });
    eventController->get<sf::Event::KeyPressed>().registerHandler(
        sf::Keyboard::Escape, [&window](const sf::Event::KeyEvent&) { window->close(); });

    auto stageController = std::make_unique<ScreenController>(*eventController, *window);

    GameController game{std::move(window), std::move(eventController), std::move(stageController)};
    game.run();

    return EXIT_SUCCESS;
}