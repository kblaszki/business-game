/* Created by kblaszki */

#include "fakes/NullDrawer.hpp"

#include <entities/Ball.hpp>
#include <entities/CollidableI.hpp>
#include <entities/EntityI.hpp>
#include <entities/Paddle.hpp>
#include <screens/GameplayScreen.hpp>
#include <screens/ScreenStack.hpp>

#include <LevelDescriptor.hpp>
#include <LevelId.hpp>
#include <World.hpp>
#include <gtest/gtest.h>
#include <makeWorld.hpp>
#include <memory>

namespace
{
class DummyEntity : public EntityI
{
public:
    void fixedUpdate(sf::Time) override {}

    void draw(DrawerI&) const override {}
};
} // namespace

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
    const auto* const paddle = dynamic_cast<const CollidableI*>(world.objectAt(0));
    ASSERT_NE(paddle, nullptr);
    EXPECT_EQ(paddle->position(), (sf::Vector2f{590.f, 680.f}));
}

TEST(WorldShould, spawnNonCollidableEntityWithoutBounds)
{
    World world;
    world.spawn(std::make_unique<DummyEntity>());

    EXPECT_EQ(world.objectCount(), 1u);
    EXPECT_EQ(dynamic_cast<CollidableI*>(world.objectAt(0)), nullptr);
    EXPECT_TRUE(world.objectAt(0)->alive());

    NullDrawer drawer;
    world.draw(drawer);
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
