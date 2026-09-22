#include <Game.hpp>
#include <screen/ScreenI.hpp>
#include <time/ClockSFML.hpp>
#include <window/WindowSFML.hpp>

namespace
{
class DummyScreen : public ScreenI
{
public:
    bool handleEvent(const sf::Event&) override
    {
        return false;
    }

    void update(sf::Time) override
    {
    }

    void draw(DrawerI&) override
    {
    }

    [[nodiscard]] bool blocksUpdate() const override
    {
        return false;
    }

    [[nodiscard]] bool blocksDraw() const override
    {
        return false;
    }
};
}

int main()
{
    WindowSFML window{Game::DESIGN_SIZE, "Business game"};
    ClockSFML clock;
    DummyScreen screen;
    Game{window, clock, screen}.run();
    return 0;
}
