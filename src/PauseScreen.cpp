/* Created by kblaszki */

#include "PauseScreen.hpp"

#include "MainMenuScreen.hpp"
#include "ScreenStack.hpp"

#include <memory>

PauseScreen::PauseScreen(ScreenStack& stack)
    : m_stack(stack)
{
}

bool PauseScreen::handleEvent(const sf::Event&)
{
    return false;
}

bool PauseScreen::handleAction(Action action)
{
    if(action == Action::Pause || action == Action::Cancel)
    {
        m_stack.requestPop();
        return true;
    }

    if(action == Action::Confirm)
    {
        m_stack.requestPop();
        m_stack.requestReplace(std::make_unique<MainMenuScreen>(m_stack));
        return true;
    }

    return false;
}

void PauseScreen::update(sf::Time) {}

bool PauseScreen::blocksUpdate() const
{
    return true;
}

bool PauseScreen::blocksDraw() const
{
    return false;
}
