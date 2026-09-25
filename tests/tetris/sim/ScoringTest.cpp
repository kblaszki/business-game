#include <cmath>
#include <gtest/gtest.h>
#include <tetris/sim/Scoring.hpp>

using sgl::tetris::applyHardDrop;
using sgl::tetris::applyLineClear;
using sgl::tetris::applySoftDrop;
using sgl::tetris::gravityInterval;
using sgl::tetris::lineClearPoints;
using sgl::tetris::Score;

TEST(ScoringTest, lineClearTable)
{
    EXPECT_EQ(lineClearPoints(1, 1), 100u);
    EXPECT_EQ(lineClearPoints(2, 1), 300u);
    EXPECT_EQ(lineClearPoints(3, 1), 500u);
    EXPECT_EQ(lineClearPoints(4, 1), 800u);
    EXPECT_EQ(lineClearPoints(4, 3), 2400u);
    EXPECT_EQ(lineClearPoints(0, 1), 0u);
}

TEST(ScoringTest, softAndHardDropPoints)
{
    Score soft{};
    applySoftDrop(soft, 5);
    EXPECT_EQ(soft.points, 5u);

    Score hard{};
    applyHardDrop(hard, 5);
    EXPECT_EQ(hard.points, 10u);
}

TEST(ScoringTest, levelUpEveryTenLines)
{
    Score score{};
    EXPECT_EQ(score.level, 1u);

    applyLineClear(score, 4);
    applyLineClear(score, 4);
    EXPECT_EQ(score.lines, 8u);
    EXPECT_EQ(score.level, 1u);

    applyLineClear(score, 2);
    EXPECT_EQ(score.lines, 10u);
    EXPECT_EQ(score.level, 2u);

    applyLineClear(score, 4);
    applyLineClear(score, 4);
    applyLineClear(score, 2);
    EXPECT_EQ(score.lines, 20u);
    EXPECT_EQ(score.level, 3u);
}

TEST(ScoringTest, gravityCurveLevel1And15)
{
    EXPECT_FLOAT_EQ(gravityInterval(1).count(), 1.f);
    const float expected15 = static_cast<float>(std::pow(0.8 - 14.0 * 0.007, 14.0));
    EXPECT_NEAR(gravityInterval(15).count(), expected15, 1e-6f);
}
