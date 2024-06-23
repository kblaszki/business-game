/* Created by kblaszki */

#include <controllers/EventController.hpp>
#include <controllers/GameController.hpp>
#include <core/WindowSFML.hpp>
#include <managers/ClosedEventManager.hpp>
#include <managers/KeyPressedManager.hpp>

int main()
{
    auto window = std::make_unique<WindowSFML>();
    auto eventController = std::make_unique<EventController>(*window);
    eventController->emplace<ClosedEventManager>();
    eventController->emplace<KeyPressedManager>();

    eventController->get<sf::Event::Closed>().registerHandler([&window]() { window->close(); });
    eventController->get<sf::Event::KeyPressed>().registerHandler(
        sf::Keyboard::Escape, [&window](const sf::Event::KeyEvent&) { window->close(); });

    GameController game{std::move(window), std::move(eventController)};
    game.run();

    return EXIT_SUCCESS;
}