/* Created by kblaszki */

#include <controllers/EventController.hpp>
#include <controllers/GameController.hpp>
#include <core/WindowSFML.hpp>

int main()
{
    auto window = std::make_unique<WindowSFML>();
    auto eventController = std::make_unique<EventController>(*window);
    GameController game{std::move(window), std::move(eventController)};
    game.run();

    return EXIT_SUCCESS;
}