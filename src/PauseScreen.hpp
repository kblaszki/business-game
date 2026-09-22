/* Created by kblaszki */
#pragma once

#include <IScreen.hpp>

class ScreenStack;

class PauseScreen : public IScreen
{
public:
    explicit PauseScreen(ScreenStack& stack);

    bool handleEvent(const sf::Event& event) override;
    bool handleAction(Action action) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
    bool blocksUpdate() const override;
    bool blocksDraw() const override;
    bool isPauseOverlay() const override
    {
        return true;
    }

private:
    ScreenStack& m_stack;
};
