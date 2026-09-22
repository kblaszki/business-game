#include "ScreenStack.hpp"

void ScreenStack::push(std::unique_ptr<ScreenI> screen)
{
    enqueue(Command{CommandType::Push, std::move(screen)});
}

void ScreenStack::pop()
{
    enqueue(Command{CommandType::Pop, {}});
}

void ScreenStack::replace(std::unique_ptr<ScreenI> screen)
{
    enqueue(Command{CommandType::Replace, std::move(screen)});
}

bool ScreenStack::handleEvent(const sf::Event& event)
{
    dispatching = true;
    bool consumed{false};

    for(std::size_t i = screens.size(); i > 0; --i)
    {
        ScreenI& screen = *screens[i - 1];
        consumed = screen.handleEvent(event);
        if(consumed || screen.blocksUpdate())
        {
            break;
        }
    }

    dispatching = false;
    return consumed;
}

void ScreenStack::update(sf::Time dt)
{
    dispatching = true;

    for(std::size_t i = screens.size(); i > 0; --i)
    {
        ScreenI& screen = *screens[i - 1];
        screen.update(dt);
        if(screen.blocksUpdate())
        {
            break;
        }
    }

    dispatching = false;
}

void ScreenStack::draw(DrawerI& drawer)
{
    dispatching = true;

    if(!screens.empty())
    {
        std::size_t start{0};
        for(std::size_t i = screens.size(); i > 0; --i)
        {
            if(screens[i - 1]->blocksDraw())
            {
                start = i - 1;
                break;
            }
        }

        for(std::size_t i = start; i < screens.size(); ++i)
        {
            screens[i]->draw(drawer);
        }
    }

    dispatching = false;
    applyCommands();
}

bool ScreenStack::empty() const
{
    return screens.empty();
}

std::size_t ScreenStack::size() const
{
    return screens.size();
}

bool ScreenStack::blocksUpdate() const
{
    return !screens.empty() && screens.back()->blocksUpdate();
}

void ScreenStack::enqueue(Command command)
{
    commands.push_back(std::move(command));
    if(!dispatching)
    {
        applyCommands();
    }
}

void ScreenStack::applyCommands()
{
    for(Command& command : commands)
    {
        switch(command.type)
        {
            case CommandType::Push:
                if(command.screen)
                {
                    screens.push_back(std::move(command.screen));
                }
                break;
            case CommandType::Pop:
                if(!screens.empty())
                {
                    screens.pop_back();
                }
                break;
            case CommandType::Replace:
                if(!screens.empty())
                {
                    screens.pop_back();
                }
                if(command.screen)
                {
                    screens.push_back(std::move(command.screen));
                }
                break;
        }
    }

    commands.clear();
}
