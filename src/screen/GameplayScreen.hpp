#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cstdint>
#include <screen/ScreenI.hpp>
#include <string>
#include <world/LevelId.hpp>
#include <world/World.hpp>

class ScreenStack;

class GameplayScreen : public ScreenI
{
public:
    explicit GameplayScreen(ScreenStack& screens, LevelId id = LevelId::Stage1);

    bool handleEvent(const sf::Event& event) override;
    bool handleAction(Action action) override;
    void update(sf::Time dt) override;
    void draw(DrawerI& drawer) override;
    [[nodiscard]] bool blocksUpdate() const override;
    [[nodiscard]] bool blocksDraw() const override;
    [[nodiscard]] bool acceptsPauseOverlay() const override;
    [[nodiscard]] std::uint32_t tickCount() const;
    [[nodiscard]] LevelId levelId() const;
    [[nodiscard]] std::string bannerTitleText() const;
    [[nodiscard]] World& world();
    [[nodiscard]] const World& world() const;

private:
    void applyPaddleInput();
    void refreshHud();
    void restart();
    void advanceStage();

    ScreenStack& screens;
    LevelId level;
    World simulated;
    sf::Font font;
    sf::Text scoreLabel;
    sf::Text livesLabel;
    sf::Text powerLabel;
    sf::RectangleShape banner;
    sf::Text bannerTitle;
    sf::Text bannerHint;
    bool leftHeld{false};
    bool rightHeld{false};
};
