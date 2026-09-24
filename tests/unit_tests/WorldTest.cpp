#include <gtest/gtest.h>
#include <time/FixedTimestep.hpp>
#include <world/World.hpp>

TEST(WorldShould, clampPaddleInsideDesignWidth)
{
    World world;
    world.setPaddleSpeed(20000.f);
    for(int i = 0; i < 60; ++i)
    {
        world.fixedUpdate(FixedTimestep::tick);
    }

    EXPECT_GE(world.paddle().position().x, 0.f);
    EXPECT_LE(world.paddle().position().x + world.paddle().size().x, 1280.f);
}

TEST(WorldShould, bounceBallOffLeftWall)
{
    World world;
    world.placeBall({2.f, 300.f}, {-240.f, 0.f}, false);
    world.fixedUpdate(FixedTimestep::tick);

    EXPECT_GE(world.ball().position().x, 0.f);
    EXPECT_GT(world.ball().velocity().x, 0.f);
}

TEST(WorldShould, breakBrickAndRaiseScore)
{
    World world;
    world.addBrick({200.f, 80.f}, sf::Color::Red);
    world.placeBall({247.f, 96.f}, {0.f, -300.f}, false);
    world.fixedUpdate(FixedTimestep::tick);

    EXPECT_FALSE(world.brickAt(0).alive());
    EXPECT_EQ(world.score(), 10u);
}

TEST(WorldShould, bounceDownWhenHittingBrickFromBelow)
{
    World world;
    world.addBrick({200.f, 80.f}, sf::Color::Red);
    world.placeBall({247.f, 100.f}, {0.f, -300.f}, false);
    world.fixedUpdate(FixedTimestep::tick);

    EXPECT_FALSE(world.brickAt(0).alive());
    EXPECT_GT(world.ball().velocity().y, 0.f);
}

TEST(WorldShould, loseLifeAndRestickWhenBallExitsBottom)
{
    World world;
    world.placeBall({400.f, 730.f}, {0.f, 240.f}, false);
    world.fixedUpdate(FixedTimestep::tick);

    EXPECT_EQ(world.lives(), 2u);
    EXPECT_TRUE(world.ball().stuck());
    EXPECT_FALSE(world.lost());
}

TEST(WorldShould, winWhenLastBrickDies)
{
    World world;
    world.addBrick({200.f, 80.f}, sf::Color::Red);
    world.placeBall({247.f, 96.f}, {0.f, -300.f}, false);
    world.fixedUpdate(FixedTimestep::tick);

    EXPECT_TRUE(world.won());
}

TEST(WorldShould, alwaysAdvanceTickCount)
{
    World world;
    world.fixedUpdate(FixedTimestep::tick);
    world.fixedUpdate(FixedTimestep::tick);
    EXPECT_EQ(world.tickCount(), 2u);
}

TEST(WorldShould, widePowerUpWidensPaddle)
{
    World world;
    world.applyPowerUp(PowerUpKind::Wide);

    EXPECT_GT(world.paddle().size().x, 120.f);
}

TEST(WorldShould, extraLifeIncreasesLives)
{
    World world;
    world.applyPowerUp(PowerUpKind::ExtraLife);

    EXPECT_EQ(world.lives(), 4u);
}

TEST(WorldShould, missedCapsuleDisappears)
{
    World world;
    world.addBrick({200.f, 80.f}, sf::Color::Red);
    world.killBrick(0);

    ASSERT_EQ(world.powerUpCount(), 1u);

    for(int i = 0; i < 600 && world.powerUpCount() > 0; ++i)
    {
        world.fixedUpdate(FixedTimestep::tick);
    }

    EXPECT_EQ(world.powerUpCount(), 0u);
}
