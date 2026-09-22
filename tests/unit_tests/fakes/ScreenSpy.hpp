#pragma once

#include <screen/ScreenI.hpp>

#include <cstdint>

class ScreenSpy : public ScreenI
{
public:
    bool handleEvent(const sf::Event&) override
    {
        ++handleEventCount;
        return false;
    }

    void update(sf::Time dt) override
    {
        ++updateCount;
        lastDt = dt;
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
    std::uint32_t updateCount{};
    std::uint32_t drawCount{};
    bool updateBlocked{false};
    bool drawBlocked{false};
    sf::Time lastDt{};
};
