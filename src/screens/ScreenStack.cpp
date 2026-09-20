/* Created by kblaszki */

#include "ScreenStack.hpp"

#include "PauseScreen.hpp"

#include <cassert>

void ScreenStack::requestPush(std::unique_ptr<ScreenI> screen)
{
    assert(screen);
    if(!screen)
    {
        return;
    }

    m_commands.push_back(Command{CommandType::Push, std::move(screen)});
}

void ScreenStack::requestPop()
{
    m_commands.push_back(Command{CommandType::Pop, nullptr});
}

void ScreenStack::requestReplace(std::unique_ptr<ScreenI> screen)
{
    assert(screen);
    if(!screen)
    {
        return;
    }

    m_commands.push_back(Command{CommandType::Replace, std::move(screen)});
}

void ScreenStack::requestClose()
{
    m_closeRequested = true;
}

void ScreenStack::requestPauseOverlay()
{
    if(pauseIsTop())
    {
        return;
    }

    for(const auto& command: m_commands)
    {
        if(command.type == CommandType::Push && command.screen != nullptr && command.screen->isPauseOverlay())
        {
            return;
        }
    }

    if(!gameplayIsTop())
    {
        return;
    }

    requestPush(std::make_unique<PauseScreen>(*this));
}

void ScreenStack::applyCommands()
{
    for(auto& command: m_commands)
    {
        switch(command.type)
        {
            case CommandType::Push:
            {
                if(command.screen)
                {
                    m_screens.push_back(std::move(command.screen));
                }
                break;
            }
            case CommandType::Pop:
            {
                if(!m_screens.empty())
                {
                    m_screens.pop_back();
                }
                break;
            }
            case CommandType::Replace:
            {
                if(!m_screens.empty() && command.screen)
                {
                    m_screens.pop_back();
                    m_screens.push_back(std::move(command.screen));
                }
                break;
            }
        }
    }

    m_commands.clear();
}

bool ScreenStack::handleEvent(const sf::Event& event)
{
    for(auto i = m_screens.size(); i > 0; --i)
    {
        ScreenI* const screen = m_screens[i - 1].get();
        const bool consumed = screen->handleEvent(event);
        if(consumed || screen->blocksUpdate())
        {
            return consumed;
        }
    }

    return false;
}

bool ScreenStack::handleAction(Action action)
{
    for(auto i = m_screens.size(); i > 0; --i)
    {
        ScreenI* const screen = m_screens[i - 1].get();
        const bool consumed = screen->handleAction(action);
        if(consumed || screen->blocksUpdate())
        {
            return consumed;
        }
    }

    return false;
}

void ScreenStack::update(sf::Time dt)
{
    for(auto i = m_screens.size(); i > 0; --i)
    {
        ScreenI* const screen = m_screens[i - 1].get();
        screen->update(dt);
        if(screen->blocksUpdate())
        {
            return;
        }
    }
}

void ScreenStack::draw(DrawerI& drawer)
{
    if(m_screens.empty())
    {
        return;
    }

    for(auto i = drawStartIndex(); i < m_screens.size(); ++i)
    {
        m_screens[i]->draw(drawer);
    }
}

bool ScreenStack::empty() const
{
    return m_screens.empty();
}

std::size_t ScreenStack::size() const
{
    return m_screens.size();
}

bool ScreenStack::closeRequested() const
{
    return m_closeRequested;
}

ScreenI* ScreenStack::top() const
{
    if(m_screens.empty())
    {
        return nullptr;
    }

    return m_screens.back().get();
}

bool ScreenStack::gameplayIsTop() const
{
    return top() != nullptr && top()->isGameplay();
}

bool ScreenStack::pauseIsTop() const
{
    return top() != nullptr && top()->isPauseOverlay();
}

std::size_t ScreenStack::drawStartIndex() const
{
    std::size_t start = 0;
    for(auto i = m_screens.size(); i > 0; --i)
    {
        if(m_screens[i - 1]->blocksDraw())
        {
            start = i - 1;
            break;
        }
    }
    return start;
}
