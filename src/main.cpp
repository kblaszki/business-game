/* Created by kblaszki */

#include "controllers/GameController.hpp"
#include "core/WindowSFML.hpp"

int main()
{
    GameController game{std::make_unique<WindowSFML>()};
    game.run();

    return EXIT_SUCCESS;
}