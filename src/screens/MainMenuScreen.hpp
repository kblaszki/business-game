/* Created by kblaszki */
#pragma once

#include "ScreenI.hpp"

#include <entities/Button.hpp>
#include <entities/EntityI.hpp>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>

class ScreenUpdaterI;

class MainMenuScreen : public ScreenI
{
public:
    enum class Hover
    {
        None,
        Start,
        Exit
    };

    static constexpr sf::FloatRect startButton{{440.f, 250.f}, {400.f, 80.f}};
    static constexpr sf::FloatRect exitButton{{440.f, 390.f}, {400.f, 80.f}};

    explicit MainMenuScreen(ScreenUpdaterI& stack);

    bool handleEvent(const sf::Event& event) override;
    bool handleAction(Action action) override;
    void update(sf::Time dt) override;
    void draw(DrawerI& drawer) override;
    bool blocksUpdate() const override;
    bool blocksDraw() const override;

    [[nodiscard]] Hover hover() const;

private:
    void startGame();
    void setHoverFrom(sf::Vector2i pixel);

    ScreenUpdaterI& m_stack;
    std::unique_ptr<EntityI> m_panel;
    Button m_start;
    Button m_exit;
};
