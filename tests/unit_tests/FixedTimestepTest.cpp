#include <time/FixedTimestep.hpp>

#include <gtest/gtest.h>

TEST(FixedTimestepShould, drainOneTickFromExactTick)
{
    FixedTimestep step;
    EXPECT_EQ(step.drain(FixedTimestep::tick), 1u);
    EXPECT_LT(step.accumulator, FixedTimestep::tick);
}

TEST(FixedTimestepShould, capHitchAtAccumulatorMax)
{
    FixedTimestep step;
    const auto steps = step.drain(sf::seconds(10.f));
    const auto maxSteps =
        static_cast<std::uint32_t>(FixedTimestep::accumulatorMax.asSeconds() / FixedTimestep::tick.asSeconds());

    EXPECT_EQ(steps, maxSteps);
    EXPECT_LE(step.accumulator, FixedTimestep::accumulatorMax);
    EXPECT_LT(step.accumulator, FixedTimestep::tick);
}
