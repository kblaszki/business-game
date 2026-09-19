/* Created by kblaszki */

#include "fakes/SpyScreen.hpp"

#include <SFML/Window/Event.hpp>

#include <FixedTimestep.hpp>
#include <gtest/gtest.h>

TEST(IScreenDummyShould, exposeNonBlockingFlagsAndIgnoreEvents)
{
    SpyScreen screen;
    const sf::Event event{sf::Event::Closed{}};

    EXPECT_FALSE(screen.blocksUpdate());
    EXPECT_FALSE(screen.blocksDraw());
    EXPECT_FALSE(screen.handleEvent(event));
    EXPECT_EQ(screen.handleEventCount, 1u);
}

TEST(IScreenDummyShould, increaseUpdateCountWhenDrainResultIsApplied)
{
    SpyScreen screen;
    FixedTimestep timestep;
    const auto ticks = timestep.drain(FixedTimestep::tick);
    for(auto i = 0u; i < ticks; ++i)
    {
        screen.update(FixedTimestep::tick);
    }

    EXPECT_EQ(screen.updateCount, 1u);
}
