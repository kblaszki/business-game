#include <Game.hpp>
#include <screen/ScreenI.hpp>
#include <screen/ScreenStack.hpp>
#include <time/ClockSFML.hpp>
#include <window/WindowSFML.hpp>

#include <memory>

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
    ScreenStack screens;
    screens.push(std::make_unique<DummyScreen>());
    Game{window, clock, screens}.run();
    return 0;
}
