/* Created by kblaszki */

#include <RectCollision.hpp>
#include <gtest/gtest.h>

TEST(RectCollisionShould, reportIntersectionWhenRectsOverlap)
{
    const sf::FloatRect a{{0.f, 0.f}, {10.f, 10.f}};
    const sf::FloatRect b{{5.f, 5.f}, {10.f, 10.f}};

    EXPECT_TRUE(intersects(a, b));
    EXPECT_FALSE(intersects(a, sf::FloatRect{{20.f, 20.f}, {4.f, 4.f}}));
}

TEST(RectCollisionShould, flipShallowerAxisOnAabbHit)
{
    sf::Vector2f velocity{100.f, 10.f};
    const sf::FloatRect ball{{8.f, 0.f}, {16.f, 16.f}};
    const sf::FloatRect block{{20.f, 0.f}, {80.f, 30.f}};

    bounceBallOffAabb(velocity, ball, block);

    EXPECT_LT(velocity.x, 0.f);
    EXPECT_FLOAT_EQ(velocity.y, 10.f);
}

TEST(RectCollisionShould, keepSpeedAndAimUpOnPaddleHit)
{
    sf::Vector2f velocity{0.f, 420.f};
    const sf::FloatRect ball{{640.f, 660.f}, {16.f, 16.f}};
    const sf::FloatRect paddle{{590.f, 680.f}, {100.f, 20.f}};

    bounceBallOffPaddle(velocity, ball, paddle);

    const float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    EXPECT_NEAR(speed, 420.f, 0.01f);
    EXPECT_LT(velocity.y, 0.f);
}

TEST(RectCollisionShould, anglePaddleHitTowardTheStruckSide)
{
    sf::Vector2f velocity{0.f, 420.f};
    const sf::FloatRect ball{{670.f, 660.f}, {16.f, 16.f}};
    const sf::FloatRect paddle{{590.f, 680.f}, {100.f, 20.f}};

    bounceBallOffPaddle(velocity, ball, paddle);

    EXPECT_GT(velocity.x, 0.f);
    EXPECT_LT(velocity.y, 0.f);
}
