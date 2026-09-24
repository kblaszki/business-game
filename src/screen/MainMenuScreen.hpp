#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <screen/ScreenI.hpp>
#include <world/BreakoutArt.hpp>

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
    [[nodiscard]] sf::FloatRect startButtonBounds() const;

private:
    void startGame();
    void paintButtons();
    [[nodiscard]] bool hitStart(sf::Vector2f point) const;
    [[nodiscard]] bool hitQuit(sf::Vector2f point) const;

    ScreenStack& screens;
    BreakoutArt art;
    sf::Font font;
    sf::RectangleShape startButton;
    sf::RectangleShape quitButton;
    sf::Text title;
    sf::Text startLabel;
    sf::Text quitLabel;
    bool startHover{false};
    bool quitHover{false};
};
