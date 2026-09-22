/* Created by kblaszki */

#include <entities/Ball.hpp>

#include <mocks/window/ScreenRendererMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

TEST(BallShould, moveByVelocityTimesDeltaTime)
{
    Ball ball;
    ball.setPosition({100.f, 200.f});
    ball.setVelocity({40.f, -80.f});

    ball.update(0.5f);

    EXPECT_FLOAT_EQ(120.f, ball.getPosition().x);
    EXPECT_FLOAT_EQ(160.f, ball.getPosition().y);
}

TEST(BallShould, resetAbovePaddle)
{
    Ball ball;
    ball.resetAbove({500.f, 680.f}, {100.f, 20.f});

    EXPECT_FLOAT_EQ(550.f, ball.getPosition().x);
    EXPECT_FLOAT_EQ(680.f - Ball::RADIUS - 2.f, ball.getPosition().y);
    EXPECT_LT(ball.getVelocity().y, 0.f);
}

TEST(BallShould, drawThroughDrawer)
{
    Ball ball;
    StrictMock<ScreenRendererMock> renderer;
    EXPECT_CALL(renderer, draw(_, _)).Times(1);
    ball.draw(renderer);
}
