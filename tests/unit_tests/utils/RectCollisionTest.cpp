/* Created by kblaszki */

#include <utils/RectCollision.hpp>

#include <gtest/gtest.h>

TEST(RectCollisionShould, detectIntersection)
{
    const sf::FloatRect a{{0.f, 0.f}, {10.f, 10.f}};
    const sf::FloatRect b{{5.f, 5.f}, {10.f, 10.f}};
    const sf::FloatRect c{{20.f, 20.f}, {5.f, 5.f}};

    EXPECT_TRUE(intersects(a, b));
    EXPECT_FALSE(intersects(a, c));
}

TEST(RectCollisionShould, bounceOffVerticalAndHorizontalWalls)
{
    sf::Vector2f velocity{3.f, -4.f};
    bounceBallOffVerticalWall(velocity);
    EXPECT_FLOAT_EQ(-3.f, velocity.x);
    EXPECT_FLOAT_EQ(-4.f, velocity.y);

    bounceBallOffHorizontalWall(velocity);
    EXPECT_FLOAT_EQ(-3.f, velocity.x);
    EXPECT_FLOAT_EQ(4.f, velocity.y);
}

TEST(RectCollisionShould, bounceOffAabbPreferringShallowerOverlapAxis)
{
    sf::Vector2f velocity{10.f, 10.f};
    const sf::FloatRect ball{{8.f, 0.f}, {10.f, 10.f}};
    const sf::FloatRect block{{10.f, 0.f}, {10.f, 10.f}};

    bounceBallOffAabb(velocity, ball, block);
    EXPECT_FLOAT_EQ(-10.f, velocity.x);
    EXPECT_FLOAT_EQ(10.f, velocity.y);
}

TEST(RectCollisionShould, bounceOffPaddleSendsBallUpward)
{
    sf::Vector2f velocity{0.f, 100.f};
    const sf::FloatRect ball{{140.f, 90.f}, {16.f, 16.f}};
    const sf::FloatRect paddle{{100.f, 100.f}, {100.f, 20.f}};

    bounceBallOffPaddle(velocity, ball, paddle);
    EXPECT_LT(velocity.y, 0.f);
}
