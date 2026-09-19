/* Created by kblaszki */
#pragma once

#include <IScreen.hpp>
#include <LevelId.hpp>
#include <World.hpp>
#include <cstdint>

class ScreenStack;

class GameplayScreen : public IScreen
{
public:
    explicit GameplayScreen(ScreenStack& stack, LevelId levelId);

    bool handleEvent(const sf::Event& event) override;
    bool handleAction(Action action) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
    bool blocksUpdate() const override;
    bool blocksDraw() const override;

    [[nodiscard]] std::uint32_t tickCount() const;
    [[nodiscard]] std::uint32_t drawCount() const;
    [[nodiscard]] std::uint32_t handleActionCount() const;
    [[nodiscard]] sf::Vector2f dummyPosition() const;

private:
    ScreenStack& m_stack;
    World m_world;
    std::uint32_t m_tickCount{0};
    std::uint32_t m_drawCount{0};
    std::uint32_t m_handleActionCount{0};
};
