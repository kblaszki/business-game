#include <SFML/Graphics/RectangleShape.hpp>

#include <gtest/gtest.h>
#include <sgl/render/DrawCommand.hpp>
#include <sgl/sfml/SfmlDraw.hpp>

TEST(SfmlDrawTest, rotationAroundCenterKeepsTopLeftAtZero)
{
    const sgl::RectCmd cmd{
        .rect = {.pos = {0.f, 0.f}, .size = {40.f, 20.f}},
        .fill = {255, 255, 255, 255},
        .rotationDeg = 0.f,
    };
    const sf::RectangleShape unrotated = sgl::sfml::toShape(cmd);
    EXPECT_FLOAT_EQ(unrotated.getOrigin().x, 20.f);
    EXPECT_FLOAT_EQ(unrotated.getOrigin().y, 10.f);
    EXPECT_FLOAT_EQ(unrotated.getPosition().x, 20.f);
    EXPECT_FLOAT_EQ(unrotated.getPosition().y, 10.f);

    const sf::Vector2f topLeft = unrotated.getTransform().transformPoint({0.f, 0.f});
    EXPECT_FLOAT_EQ(topLeft.x, 0.f);
    EXPECT_FLOAT_EQ(topLeft.y, 0.f);

    sgl::RectCmd rotated = cmd;
    rotated.rotationDeg = 90.f;
    const sf::RectangleShape shape = sgl::sfml::toShape(rotated);
    EXPECT_FLOAT_EQ(shape.getOrigin().x, 20.f);
    EXPECT_FLOAT_EQ(shape.getOrigin().y, 10.f);
    EXPECT_FLOAT_EQ(shape.getPosition().x, 20.f);
    EXPECT_FLOAT_EQ(shape.getPosition().y, 10.f);
    EXPECT_FLOAT_EQ(shape.getRotation().asDegrees(), 90.f);

    // Center of the unrotated bounds stays fixed under rotation.
    const sf::Vector2f center = shape.getTransform().transformPoint({20.f, 10.f});
    EXPECT_NEAR(center.x, 20.f, 1e-4f);
    EXPECT_NEAR(center.y, 10.f, 1e-4f);
}
