#include <Game.hpp>
#include <window/WindowSFML.hpp>

int main()
{
    WindowSFML window{Game::DESIGN_SIZE, "Business game"};
    Game{window}.run();
    return 0;
}
