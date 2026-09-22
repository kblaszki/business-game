/* Created by kblaszki */

#include "fakes/SpyScreen.hpp"

#include <FixedTimestep.hpp>
#include <gtest/gtest.h>

TEST(FixedTimestepShould, drainOneTickFromExactlyOneSixtieth)
{
    FixedTimestep timestep;
    EXPECT_EQ(timestep.drain(FixedTimestep::tick), 1u);
    EXPECT_EQ(timestep.accumulator, sf::Time{});
}

TEST(FixedTimestepShould, drainTwoTicksFromTwoSixtieths)
{
    FixedTimestep timestep;
    EXPECT_EQ(timestep.drain(FixedTimestep::tick + FixedTimestep::tick), 2u);
    EXPECT_EQ(timestep.accumulator, sf::Time{});
}

TEST(FixedTimestepShould, capHugeDeltaSoTicksStayAtMostFifteen)
{
    FixedTimestep timestep;
    const auto ticks = timestep.drain(sf::seconds(10.f));
    const auto maxTicks =
        static_cast<std::uint32_t>(FixedTimestep::accumulatorMax.asSeconds() / FixedTimestep::tick.asSeconds());

    EXPECT_LE(ticks, 15u);
    EXPECT_EQ(ticks, maxTicks);
    EXPECT_LE(timestep.accumulator, FixedTimestep::tick);
}

TEST(FixedTimestepShould, keepRemainderAfterPartialTick)
{
    FixedTimestep timestep;
    const sf::Time oneAndAHalf = sf::seconds(1.5f / 60.f);

    EXPECT_EQ(timestep.drain(oneAndAHalf), 1u);
    EXPECT_GT(timestep.accumulator, sf::Time{});
    EXPECT_LT(timestep.accumulator, FixedTimestep::tick);

    EXPECT_EQ(timestep.drain(FixedTimestep::tick - timestep.accumulator), 1u);
    EXPECT_EQ(timestep.accumulator, sf::Time{});
}

TEST(FixedTimestepShould, increaseSpyScreenUpdateCountForDrainedTicks)
{
    SpyScreen screen;
    FixedTimestep timestep;
    const auto ticks = timestep.drain(FixedTimestep::tick + FixedTimestep::tick);
    for(auto i = 0u; i < ticks; ++i)
    {
        screen.update(FixedTimestep::tick);
    }

    EXPECT_EQ(ticks, 2u);
    EXPECT_EQ(screen.updateCount, 2u);
}
