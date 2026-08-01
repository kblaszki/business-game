/* Created by kblaszki */

#include <entities/Brick.hpp>

#include <mocks/window/ScreenRendererMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

TEST(BrickShould, reportBoundsFromPositionAndSize)
{
    Brick brick{{100.f, 50.f}, sf::Color::Red};
    const sf::FloatRect bounds = brick.getBounds();

    EXPECT_FLOAT_EQ(100.f, bounds.position.x);
    EXPECT_FLOAT_EQ(50.f, bounds.position.y);
    EXPECT_FLOAT_EQ(Brick::WIDTH, bounds.size.x);
    EXPECT_FLOAT_EQ(Brick::HEIGHT, bounds.size.y);
}

TEST(BrickShould, stopDrawingAfterDestroy)
{
    Brick brick{{0.f, 0.f}, sf::Color::Blue};
    EXPECT_FALSE(brick.isDestroyed());

    StrictMock<ScreenRendererMock> renderer;
    EXPECT_CALL(renderer, draw(_, _)).Times(1);
    brick.draw(renderer);

    brick.destroy();
    EXPECT_TRUE(brick.isDestroyed());
    brick.draw(renderer);
}
