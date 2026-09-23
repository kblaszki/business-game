#include <gtest/gtest.h>
#include <memory>
#include <time/FixedTimestep.hpp>
#include <world/GameObject.hpp>
#include <world/World.hpp>

TEST(WorldShould, advanceDummyFourPixelsPerTick)
{
    World world;
    world.spawn(std::make_unique<GameObject>(sf::Vector2f{0.f, 340.f}, sf::Vector2f{240.f, 0.f}));

    world.fixedUpdate(FixedTimestep::tick);
    world.fixedUpdate(FixedTimestep::tick);
    world.fixedUpdate(FixedTimestep::tick);

    EXPECT_EQ(world.tickCount(), 3u);
    EXPECT_FLOAT_EQ(world.objectAt(0).position().x, 12.f);
    EXPECT_FLOAT_EQ(world.objectAt(0).position().y, 340.f);
}

TEST(WorldShould, wrapOriginWhenLeavingDesignWidth)
{
    World world;
    world.spawn(std::make_unique<GameObject>(sf::Vector2f{1276.f, 340.f}, sf::Vector2f{240.f, 0.f}));

    world.fixedUpdate(FixedTimestep::tick);

    EXPECT_GE(world.objectAt(0).position().x, 0.f);
    EXPECT_LT(world.objectAt(0).position().x, 1280.f);
    EXPECT_FLOAT_EQ(world.objectAt(0).position().x, 0.f);
}

TEST(WorldShould, alwaysMoveOnFixedUpdate)
{
    World world;
    world.spawn(std::make_unique<GameObject>(sf::Vector2f{0.f, 340.f}, sf::Vector2f{240.f, 0.f}));

    world.fixedUpdate(FixedTimestep::tick);
    EXPECT_FLOAT_EQ(world.objectAt(0).position().x, 4.f);
    world.fixedUpdate(FixedTimestep::tick);
    EXPECT_FLOAT_EQ(world.objectAt(0).position().x, 8.f);
}
