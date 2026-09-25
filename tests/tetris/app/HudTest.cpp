#include <gtest/gtest.h>
#include <tetris/app/HudModel.hpp>
#include <tetris/sim/Scoring.hpp>

TEST(HudTest, showsScoreLinesLevel)
{
    sgl::tetris::Score score{.points = 1200, .lines = 14, .level = 2};
    const sgl::tetris::HudModel hud = sgl::tetris::makeHud(score);
    EXPECT_EQ(hud.score, "Score 1200");
    EXPECT_EQ(hud.lines, "Lines 14");
    EXPECT_EQ(hud.level, "Level 2");
}
