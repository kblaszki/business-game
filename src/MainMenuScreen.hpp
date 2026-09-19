/* Created by kblaszki */
#pragma once

#include <IScreen.hpp>

class ScreenStack;

class MainMenuScreen : public IScreen
{
public:
    explicit MainMenuScreen(ScreenStack& stack);

    bool handleEvent(const sf::Event& event) override;
    bool handleAction(Action action) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
    bool blocksUpdate() const override;
    bool blocksDraw() const override;

private:
    ScreenStack& m_stack;
};
