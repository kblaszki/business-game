#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include <screen/ScreenI.hpp>

class ScreenStack;

class PauseScreen : public ScreenI
{
public:
    explicit PauseScreen(ScreenStack& screens);

    bool handleEvent(const sf::Event& event) override;
    bool handleAction(Action action) override;
    void update(sf::Time dt) override;
    void draw(DrawerI& drawer) override;
    [[nodiscard]] bool blocksUpdate() const override;
    [[nodiscard]] bool blocksDraw() const override;
    [[nodiscard]] bool isPauseOverlay() const override;

private:
    ScreenStack& screens;
    sf::RectangleShape dim;
};
