#pragma once

#include <cstdint>
#include <screen/ScreenI.hpp>
#include <screen/ScreenStack.hpp>

class ScreenSpy : public ScreenI
{
public:
    ~ScreenSpy() override
    {
        if(destroyed)
        {
            *destroyed = true;
        }
    }

    bool handleEvent(const sf::Event&) override
    {
        ++handleEventCount;
        return consumeEvent;
    }

    bool handleAction(Action action) override
    {
        ++handleActionCount;
        lastAction = action;
        return consumeAction;
    }

    void update(sf::Time dt) override
    {
        ++updateCount;
        lastDt = dt;
        if(popOnUpdate && stack)
        {
            stack->pop();
        }
    }

    void draw(DrawerI&) override
    {
        ++drawCount;
    }

    [[nodiscard]] bool blocksUpdate() const override
    {
        return updateBlocked;
    }

    [[nodiscard]] bool blocksDraw() const override
    {
        return drawBlocked;
    }

    std::uint32_t handleEventCount{};
    std::uint32_t handleActionCount{};
    std::uint32_t updateCount{};
    std::uint32_t drawCount{};
    bool updateBlocked{false};
    bool drawBlocked{false};
    bool consumeEvent{false};
    bool consumeAction{false};
    Action lastAction{Action::Confirm};
    bool popOnUpdate{false};
    bool* destroyed{nullptr};
    ScreenStack* stack{nullptr};
    sf::Time lastDt{};
};
