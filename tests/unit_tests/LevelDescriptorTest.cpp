#include <gtest/gtest.h>
#include <world/LevelDescriptor.hpp>

TEST(LevelDescriptorShould, describeStage1Grid)
{
    const LevelDescriptor& desc = levelDescriptor(LevelId::Stage1);

    EXPECT_EQ(desc.id, LevelId::Stage1);
    EXPECT_EQ(desc.columns, 10u);
    EXPECT_EQ(desc.rows, 6u);
    EXPECT_EQ(desc.palette.size(), 6u);
}

TEST(LevelDescriptorShould, makeWorldWithBrickGridAndStuckBall)
{
    const World world = makeWorld(levelDescriptor(LevelId::Stage1));

    EXPECT_EQ(world.brickCount(), 60u);
    EXPECT_TRUE(world.brickAt(0).alive());
    EXPECT_TRUE(world.ball().stuck());
    EXPECT_EQ(world.lives(), 3u);
    EXPECT_EQ(world.score(), 0u);
    EXPECT_FALSE(world.won());
    EXPECT_FALSE(world.lost());
}
