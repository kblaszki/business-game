#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <screen/ScreenI.hpp>

class ScreenStack;

class MainMenuScreen : public ScreenI
{
public:
    explicit MainMenuScreen(ScreenStack& screens);

    bool handleEvent(const sf::Event& event) override;
    bool handleAction(Action action) override;
    void update(sf::Time dt) override;
    void draw(DrawerI& drawer) override;
    [[nodiscard]] bool blocksUpdate() const override;
    [[nodiscard]] bool blocksDraw() const override;

private:
    ScreenStack& screens;
    sf::Font font;
    sf::RectangleShape startBar;
    sf::Text title;
    sf::Text startHint;
    sf::Text quitHint;
};
