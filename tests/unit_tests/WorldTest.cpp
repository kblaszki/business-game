/* Created by kblaszki */

#include <Ball.hpp>
#include <GameplayScreen.hpp>
#include <LevelDescriptor.hpp>
#include <LevelId.hpp>
#include <Paddle.hpp>
#include <ScreenStack.hpp>
#include <World.hpp>
#include <gtest/gtest.h>
#include <makeWorld.hpp>

TEST(WorldShould, exposeArkanoidDescriptor)
{
    const auto& descriptor = levelDescriptor(LevelId::Arkanoid);

    EXPECT_EQ(descriptor.id, LevelId::Arkanoid);
}

TEST(WorldShould, spawnPaddleBallAndFiftyBricks)
{
    const auto world = makeWorld(levelDescriptor(LevelId::Arkanoid));

    EXPECT_EQ(world.objectCount(), 52u);
    ASSERT_NE(dynamic_cast<const Paddle*>(world.objectAt(0)), nullptr);
    ASSERT_NE(dynamic_cast<const Ball*>(world.objectAt(1)), nullptr);
    EXPECT_EQ(world.objectAt(0)->position(), (sf::Vector2f{590.f, 680.f}));
}

TEST(WorldShould, placeBallAbovePaddleAtSpawn)
{
    const auto world = makeWorld(levelDescriptor(LevelId::Arkanoid));
    const auto* const paddle = dynamic_cast<const Paddle*>(world.objectAt(0));
    const auto* const ball = dynamic_cast<const Ball*>(world.objectAt(1));
    ASSERT_NE(paddle, nullptr);
    ASSERT_NE(ball, nullptr);

    EXPECT_FLOAT_EQ(ball->position().x, paddle->position().x + paddle->size().x / 2.f);
    EXPECT_FLOAT_EQ(ball->position().y, paddle->position().y - Ball::RADIUS - 2.f);
}

TEST(WorldShould, exposePaddlePoseOnGameplayScreen)
{
    ScreenStack stack;
    GameplayScreen screen{stack, LevelId::Arkanoid};

    EXPECT_EQ(screen.paddlePosition(), (sf::Vector2f{590.f, 680.f}));
    EXPECT_EQ(screen.remainingBricks(), 50u);
    EXPECT_EQ(screen.lives(), 3);
}
