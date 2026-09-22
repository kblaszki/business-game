#include <window/WindowSFML.hpp>

#include <Game.hpp>
#include <memory>
#include <screen/MainMenuScreen.hpp>
#include <screen/ScreenStack.hpp>
#include <time/ClockSFML.hpp>

int main()
{
    WindowSFML window{Game::DESIGN_SIZE, "Business game"};
    ClockSFML clock;
    ScreenStack screens;
    screens.push(std::make_unique<MainMenuScreen>(screens));
    Game{window, clock, screens}.run();
    return 0;
}
