#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <sgl/core/Random.hpp>

TEST(RandomTest, sameSeedSameSequence)
{
    sgl::Pcg32 rng{42};
    // First five nextU32() outputs for seed 42, stream 1 (PCG-XSH-RR 64/32).
    constexpr std::array<std::uint32_t, 5> expected{
        1307692281u,
        3850602322u,
        1491967504u,
        4091771729u,
        3882238836u,
    };
    for(const std::uint32_t want: expected)
    {
        EXPECT_EQ(rng.nextU32(), want);
    }

    sgl::Pcg32 again{42};
    for(const std::uint32_t want: expected)
    {
        EXPECT_EQ(again.nextU32(), want);
    }
}

TEST(RandomTest, uniformBelowStaysInRange)
{
    sgl::Pcg32 rng{7};
    EXPECT_EQ(rng.uniformBelow(0), 0u);
    EXPECT_EQ(rng.uniformBelow(1), 0u);

    for(int i = 0; i < 10000; ++i)
    {
        const std::uint32_t v = rng.uniformBelow(10);
        EXPECT_LT(v, 10u);
    }
}

TEST(RandomTest, uniformBelowIsRoughlyUniform)
{
    constexpr int kSamples = 70000;
    constexpr int kBuckets = 7;
    constexpr double kFailAbove = 16.8;

    sgl::Pcg32 rng{12345};
    std::array<int, kBuckets> counts{};
    for(int i = 0; i < kSamples; ++i)
    {
        ++counts[rng.uniformBelow(static_cast<std::uint32_t>(kBuckets))];
    }

    const double expected = static_cast<double>(kSamples) / static_cast<double>(kBuckets);
    double chi = 0.0;
    for(const int obs: counts)
    {
        const double d = static_cast<double>(obs) - expected;
        chi += (d * d) / expected;
    }
    EXPECT_LE(chi, kFailAbove);
}
