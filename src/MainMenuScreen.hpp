/* Created by kblaszki */
#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <ScreenI.hpp>

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
    [[nodiscard]] static Hover hitTest(sf::Vector2i pixel);

    ScreenUpdaterI& m_stack;
    Hover m_hover{Hover::None};
};
