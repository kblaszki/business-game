/* Created by kblaszki */

#include <entities/Paddle.hpp>
#include <managers/KeyboardManager.hpp>

#include <mocks/window/ScreenRendererMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class PaddleShould : public Test
{
protected:
    KeyboardManager keyboardManager{};
};

TEST_F(PaddleShould, moveLeftBySpeedTimesDeltaTime)
{
    Paddle paddle{keyboardManager};

    sf::Event::KeyPressed leftPressed{};
    leftPressed.code = sf::Keyboard::Key::Left;
    keyboardManager.handleEvent(sf::Event{leftPressed});

    const sf::Vector2f before = paddle.getPosition();
    paddle.update(0.5f);

    EXPECT_FLOAT_EQ(before.x - Paddle::SPEED_PX_PER_SEC * 0.5f, paddle.getPosition().x);
    EXPECT_FLOAT_EQ(before.y, paddle.getPosition().y);
}

TEST_F(PaddleShould, moveRightBySpeedTimesDeltaTime)
{
    Paddle paddle{keyboardManager};

    sf::Event::KeyPressed rightPressed{};
    rightPressed.code = sf::Keyboard::Key::Right;
    keyboardManager.handleEvent(sf::Event{rightPressed});

    const sf::Vector2f before = paddle.getPosition();
    paddle.update(0.25f);

    EXPECT_FLOAT_EQ(before.x + Paddle::SPEED_PX_PER_SEC * 0.25f, paddle.getPosition().x);
    EXPECT_FLOAT_EQ(before.y, paddle.getPosition().y);
}

TEST_F(PaddleShould, stopInvokingHandlersAfterDestruction)
{
    {
        Paddle paddle{keyboardManager};
        sf::Event::KeyPressed leftPressed{};
        leftPressed.code = sf::Keyboard::Key::Left;
        keyboardManager.handleEvent(sf::Event{leftPressed});
        paddle.update(0.1f);
    }

    int callCount = 0;
    auto registration = keyboardManager.registerKeyHandler(
        sf::Keyboard::Key::Left, [&](const KeyStatus, const sf::Event::KeyPressed&) { ++callCount; });

    sf::Event::KeyPressed leftPressed{};
    leftPressed.code = sf::Keyboard::Key::Left;
    keyboardManager.handleEvent(sf::Event{leftPressed});

    EXPECT_EQ(1, callCount);
}

TEST_F(PaddleShould, drawThroughDrawer)
{
    Paddle paddle{keyboardManager};
    StrictMock<ScreenRendererMock> renderer;
    EXPECT_CALL(renderer, draw(_, _)).Times(1);
    paddle.draw(renderer);
}

TEST_F(PaddleShould, clampToLeftArenaEdge)
{
    Paddle paddle{keyboardManager};

    sf::Event::KeyPressed leftPressed{};
    leftPressed.code = sf::Keyboard::Key::Left;
    keyboardManager.handleEvent(sf::Event{leftPressed});

    for(int i = 0; i < 20; ++i)
    {
        paddle.update(1.f);
    }

    EXPECT_FLOAT_EQ(0.f, paddle.getPosition().x);
}

TEST_F(PaddleShould, clampToRightArenaEdge)
{
    Paddle paddle{keyboardManager};

    sf::Event::KeyPressed rightPressed{};
    rightPressed.code = sf::Keyboard::Key::Right;
    keyboardManager.handleEvent(sf::Event{rightPressed});

    for(int i = 0; i < 20; ++i)
    {
        paddle.update(1.f);
    }

    EXPECT_FLOAT_EQ(Paddle::ARENA_WIDTH - Paddle::WIDTH, paddle.getPosition().x);
}
