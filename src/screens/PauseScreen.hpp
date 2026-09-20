/* Created by kblaszki */
#pragma once

#include "ScreenI.hpp"

class ScreenUpdaterI;

class PauseScreen : public ScreenI
{
public:
    explicit PauseScreen(ScreenUpdaterI& stack);

    bool handleEvent(const sf::Event& event) override;
    bool handleAction(Action action) override;
    void update(sf::Time dt) override;
    void draw(DrawerI& drawer) override;
    bool blocksUpdate() const override;
    bool blocksDraw() const override;
    bool isPauseOverlay() const override
    {
        return true;
    }

private:
    ScreenUpdaterI& m_stack;
};
