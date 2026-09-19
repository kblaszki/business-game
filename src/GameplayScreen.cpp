/* Created by kblaszki */

#include "GameplayScreen.hpp"

#include "ScreenStack.hpp"

#include <LevelDescriptor.hpp>
#include <makeWorld.hpp>

GameplayScreen::GameplayScreen(ScreenStack& stack, LevelId levelId)
    : m_stack(stack)
    , m_world(makeWorld(levelDescriptor(levelId)))
{
}

bool GameplayScreen::handleEvent(const sf::Event&)
{
    return false;
}

bool GameplayScreen::handleAction(Action action)
{
    ++m_handleActionCount;
    if(action == Action::Pause)
    {
        m_stack.requestPauseOverlay();
        return true;
    }

    return false;
}

void GameplayScreen::update(sf::Time dt)
{
    m_world.fixedUpdate(dt);
    ++m_tickCount;
}

void GameplayScreen::draw(sf::RenderTarget& target)
{
    ++m_drawCount;
    m_world.draw(target);
}

bool GameplayScreen::blocksUpdate() const
{
    return true;
}

bool GameplayScreen::blocksDraw() const
{
    return true;
}

std::uint32_t GameplayScreen::tickCount() const
{
    return m_tickCount;
}

std::uint32_t GameplayScreen::drawCount() const
{
    return m_drawCount;
}

std::uint32_t GameplayScreen::handleActionCount() const
{
    return m_handleActionCount;
}

sf::Vector2f GameplayScreen::dummyPosition() const
{
    return m_world.dummyPosition();
}
