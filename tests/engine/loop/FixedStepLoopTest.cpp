#include <gtest/gtest.h>
#include <sgl/core/Time.hpp>
#include <sgl/loop/FixedStepLoop.hpp>

using sgl::FixedStepLoop;
using sgl::kTick;
using sgl::Seconds;
using sgl::StepResult;

TEST(FixedStepLoopShould, advanceOneStepFromExactTick)
{
    FixedStepLoop loop;
    const StepResult result = loop.advance(kTick);

    EXPECT_EQ(result.steps, 1u);
    EXPECT_FLOAT_EQ(result.alpha, 0.f);
}

TEST(FixedStepLoopShould, clampHalfSecondFrameToFifteenSteps)
{
    FixedStepLoop loop;
    const StepResult result = loop.advance(Seconds{0.5f});

    EXPECT_EQ(result.steps, 15u);
    EXPECT_GE(result.alpha, 0.f);
    EXPECT_LT(result.alpha, 1.f);
    EXPECT_NEAR(result.alpha, 0.f, 1e-5f);
}

TEST(FixedStepLoopShould, keepAlphaInHalfOpenUnitInterval)
{
    FixedStepLoop loop;
    const StepResult partial = loop.advance(Seconds{0.5f / 60.f});

    EXPECT_EQ(partial.steps, 0u);
    EXPECT_GE(partial.alpha, 0.f);
    EXPECT_LT(partial.alpha, 1.f);

    const StepResult afterSteps = loop.advance(Seconds{0.5f});
    EXPECT_GE(afterSteps.alpha, 0.f);
    EXPECT_LT(afterSteps.alpha, 1.f);
}

TEST(FixedStepLoopTest, exactStepCountOverLongRun)
{
    FixedStepLoop loop;
    std::uint32_t totalSteps{0};
    const Seconds halfTick{1.f / 120.f};
    for(int i = 0; i < 3600; ++i)
    {
        totalSteps += loop.advance(halfTick).steps;
    }
    EXPECT_EQ(totalSteps, 1800u);
}

TEST(FixedStepLoopTest, negativeFrameCountsAsZero)
{
    FixedStepLoop loop;
    loop.advance(kTick);
    const StepResult result = loop.advance(Seconds{-1.f});

    EXPECT_EQ(result.steps, 0u);
    EXPECT_FLOAT_EQ(result.alpha, 0.f);
}
