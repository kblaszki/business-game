#include <gtest/gtest.h>
#include <world/LevelDescriptor.hpp>

TEST(LevelDescriptorShould, describeSandboxDummySpawn)
{
    const LevelDescriptor& desc = levelDescriptor(LevelId::Sandbox);

    ASSERT_EQ(desc.spawns.size(), 1u);
    EXPECT_EQ(desc.id, LevelId::Sandbox);
    EXPECT_FLOAT_EQ(desc.spawns[0].position.x, 0.f);
    EXPECT_FLOAT_EQ(desc.spawns[0].position.y, 340.f);
    EXPECT_FLOAT_EQ(desc.spawns[0].velocity.x, 240.f);
    EXPECT_FLOAT_EQ(desc.spawns[0].velocity.y, 0.f);
}

TEST(LevelDescriptorShould, makeWorldWithOneDummyAtSpawnPose)
{
    const World world = makeWorld(levelDescriptor(LevelId::Sandbox));

    ASSERT_EQ(world.objectCount(), 1u);
    EXPECT_EQ(world.tickCount(), 0u);
    EXPECT_FLOAT_EQ(world.objectAt(0).position().x, 0.f);
    EXPECT_FLOAT_EQ(world.objectAt(0).position().y, 340.f);
}
