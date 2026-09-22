#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include <cstdint>
#include <screen/ScreenI.hpp>

class ScreenStack;

class GameplayScreen : public ScreenI
{
public:
    explicit GameplayScreen(ScreenStack& screens);

    bool handleEvent(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(DrawerI& drawer) override;
    [[nodiscard]] bool blocksUpdate() const override;
    [[nodiscard]] bool blocksDraw() const override;
    [[nodiscard]] std::uint32_t tickCount() const;

private:
    ScreenStack& screens;
    sf::RectangleShape dummy;
    std::uint32_t ticks{0};
};
