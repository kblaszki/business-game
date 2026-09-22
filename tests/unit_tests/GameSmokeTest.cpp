/* Created by kblaszki */

#include <Game.hpp>
#include <gtest/gtest.h>

TEST(GameSmoke, designSizeIs1280By720)
{
    EXPECT_EQ(Game::DESIGN_SIZE.x, 1280u);
    EXPECT_EQ(Game::DESIGN_SIZE.y, 720u);
}
