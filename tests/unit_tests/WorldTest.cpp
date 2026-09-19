/* Created by kblaszki */

#include <FixedTimestep.hpp>
#include <GameplayScreen.hpp>
#include <LevelDescriptor.hpp>
#include <LevelId.hpp>
#include <ScreenStack.hpp>
#include <World.hpp>
#include <gtest/gtest.h>
#include <makeWorld.hpp>

TEST(WorldShould, exposeOneSandboxSpawn)
{
    const auto& descriptor = levelDescriptor(LevelId::Sandbox);

    EXPECT_EQ(descriptor.id, LevelId::Sandbox);
    ASSERT_EQ(descriptor.spawns.size(), 1u);
    EXPECT_EQ(descriptor.spawns[0].position, (sf::Vector2f{0.f, 340.f}));
    EXPECT_EQ(descriptor.spawns[0].velocity, (sf::Vector2f{240.f, 0.f}));
}

TEST(WorldShould, makeSandboxWorldAtSpawnPose)
{
    const auto world = makeWorld(levelDescriptor(LevelId::Sandbox));

    EXPECT_EQ(world.objectCount(), 1u);
    EXPECT_EQ(world.dummyPosition(), (sf::Vector2f{0.f, 340.f}));
}

TEST(WorldShould, advanceDummyFourPixelsPerTick)
{
    auto world = makeWorld(levelDescriptor(LevelId::Sandbox));

    for(int i = 0; i < 10; ++i)
    {
        world.fixedUpdate(FixedTimestep::tick);
    }

    EXPECT_FLOAT_EQ(world.dummyPosition().x, 40.f);
    EXPECT_FLOAT_EQ(world.dummyPosition().y, 340.f);
}

TEST(WorldShould, wrapDummyXAfterThreeHundredTwentyTicks)
{
    auto world = makeWorld(levelDescriptor(LevelId::Sandbox));

    for(int i = 0; i < 320; ++i)
    {
        world.fixedUpdate(FixedTimestep::tick);
    }

    EXPECT_FLOAT_EQ(world.dummyPosition().x, 0.f);
    EXPECT_FLOAT_EQ(world.dummyPosition().y, 340.f);
}

TEST(WorldShould, keepSpawnPoseUntilFixedUpdateRuns)
{
    const auto world = makeWorld(levelDescriptor(LevelId::Sandbox));

    EXPECT_EQ(world.objectCount(), 1u);
    EXPECT_EQ(world.dummyPosition(), (sf::Vector2f{0.f, 340.f}));
}

TEST(WorldShould, spawnEverySpecAndStayEmptyWhenNone)
{
    const SpawnSpec twoSpawns[]{
        SpawnSpec{{10.f, 20.f}, {0.f, 0.f}},
        SpawnSpec{{30.f, 40.f}, {0.f, 0.f}},
    };
    const LevelDescriptor twoSpecDescriptor{
        .id = LevelId::Sandbox,
        .spawns = twoSpawns,
    };
    const auto twoObjectWorld = makeWorld(twoSpecDescriptor);
    EXPECT_EQ(twoObjectWorld.objectCount(), 2u);
    EXPECT_EQ(twoObjectWorld.dummyPosition(), (sf::Vector2f{10.f, 20.f}));

    const LevelDescriptor emptyDescriptor{
        .id = LevelId::Sandbox,
        .spawns = {},
    };
    const auto emptyWorld = makeWorld(emptyDescriptor);
    EXPECT_EQ(emptyWorld.objectCount(), 0u);
}

TEST(WorldShould, moveGameplayDummyTwentyPixelsAfterFiveUpdates)
{
    ScreenStack stack;
    GameplayScreen screen{stack, LevelId::Sandbox};

    for(int i = 0; i < 5; ++i)
    {
        screen.update(FixedTimestep::tick);
    }

    EXPECT_EQ(screen.tickCount(), 5u);
    EXPECT_FLOAT_EQ(screen.dummyPosition().x, 20.f);
    EXPECT_FLOAT_EQ(screen.dummyPosition().y, 340.f);
}
