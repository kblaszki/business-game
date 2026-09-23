#pragma once

#include <cstdint>
#include <screen/ScreenI.hpp>
#include <world/LevelId.hpp>
#include <world/World.hpp>

class ScreenStack;

class GameplayScreen : public ScreenI
{
public:
    explicit GameplayScreen(ScreenStack& screens, LevelId id = LevelId::Sandbox);

    bool handleEvent(const sf::Event& event) override;
    bool handleAction(Action action) override;
    void update(sf::Time dt) override;
    void draw(DrawerI& drawer) override;
    [[nodiscard]] bool blocksUpdate() const override;
    [[nodiscard]] bool blocksDraw() const override;
    [[nodiscard]] bool acceptsPauseOverlay() const override;
    [[nodiscard]] std::uint32_t tickCount() const;
    [[nodiscard]] const World& world() const;

private:
    ScreenStack& screens;
    World simulated;
};
