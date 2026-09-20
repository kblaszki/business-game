/* Created by kblaszki */

#include <entities/Paddle.hpp>
#include <screens/GameplayScreen.hpp>
#include <screens/MainMenuScreen.hpp>
#include <screens/ScreenStack.hpp>
#include <utils/FixedTimestep.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <LevelId.hpp>
#include <gtest/gtest.h>
#include <memory>

TEST(ArkanoidSessionShould, movePaddleLeftOneTick)
{
    ScreenStack stack;
    GameplayScreen screen{stack, LevelId::Arkanoid};

    EXPECT_TRUE(screen.handleEvent(sf::Event{sf::Event::KeyPressed{.code = sf::Keyboard::Key::Left}}));
    screen.update(FixedTimestep::tick);

    EXPECT_FLOAT_EQ(screen.paddlePosition().x, 580.f);
    EXPECT_FLOAT_EQ(screen.paddlePosition().y, 680.f);
}

TEST(ArkanoidSessionShould, clampPaddleToLeftEdge)
{
    ScreenStack stack;
    GameplayScreen screen{stack, LevelId::Arkanoid};

    ASSERT_TRUE(screen.handleEvent(sf::Event{sf::Event::KeyPressed{.code = sf::Keyboard::Key::Left}}));
    for(int i = 0; i < 80; ++i)
    {
        screen.update(FixedTimestep::tick);
    }

    EXPECT_FLOAT_EQ(screen.paddlePosition().x, 0.f);
}

TEST(ArkanoidSessionShould, loseALifeWhenBallHitsTheFloor)
{
    ScreenStack stack;
    GameplayScreen screen{stack, LevelId::Arkanoid};

    screen.dropBallForTest();
    screen.update(FixedTimestep::tick);

    EXPECT_EQ(screen.lives(), 2);
    EXPECT_FLOAT_EQ(screen.ballPosition().x, screen.paddlePosition().x + Paddle::WIDTH / 2.f);
}

TEST(ArkanoidSessionShould, replaceWithMenuAfterThreeLives)
{
    ScreenStack stack;
    auto gameplay = std::make_unique<GameplayScreen>(stack, LevelId::Arkanoid);
    stack.requestPush(std::move(gameplay));
    stack.applyCommands();

    auto* const screen = dynamic_cast<GameplayScreen*>(stack.top());
    ASSERT_NE(screen, nullptr);

    screen->loseLifeForTest();
    screen->loseLifeForTest();
    screen->loseLifeForTest();
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(stack.top()), nullptr);
}

TEST(ArkanoidSessionShould, replaceWithMenuWhenAllBricksAreGone)
{
    ScreenStack stack;
    auto gameplay = std::make_unique<GameplayScreen>(stack, LevelId::Arkanoid);
    stack.requestPush(std::move(gameplay));
    stack.applyCommands();

    auto* const screen = dynamic_cast<GameplayScreen*>(stack.top());
    ASSERT_NE(screen, nullptr);
    screen->destroyAllBricksForTest();
    EXPECT_EQ(screen->remainingBricks(), 0u);

    screen->update(FixedTimestep::tick);
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(stack.top()), nullptr);
}

TEST(ArkanoidSessionShould, consumeLeftAndRightKeyEvents)
{
    ScreenStack stack;
    GameplayScreen screen{stack, LevelId::Arkanoid};

    EXPECT_TRUE(screen.handleEvent(sf::Event{sf::Event::KeyPressed{.code = sf::Keyboard::Key::Right}}));
    EXPECT_TRUE(screen.handleEvent(sf::Event{sf::Event::KeyReleased{.code = sf::Keyboard::Key::Right}}));
    EXPECT_FALSE(screen.handleEvent(sf::Event{sf::Event::KeyPressed{.code = sf::Keyboard::Key::A}}));
}
