/* Created by kblaszki */

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include <Action.hpp>
#include <GameplayScreen.hpp>
#include <MainMenuScreen.hpp>
#include <ScreenStack.hpp>
#include <gtest/gtest.h>
#include <memory>

TEST(ScreenTransitionShould, keepMenuOnTopAfterPushAndApply)
{
    ScreenStack stack;
    stack.requestPush(std::make_unique<MainMenuScreen>(stack));
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(stack.top()), nullptr);
}

TEST(ScreenTransitionShould, replaceMenuWithGameplayOnEnter)
{
    ScreenStack stack;
    stack.requestPush(std::make_unique<MainMenuScreen>(stack));
    stack.applyCommands();

    EXPECT_TRUE(stack.handleAction(Action::Confirm));
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_NE(dynamic_cast<GameplayScreen*>(stack.top()), nullptr);
    EXPECT_EQ(dynamic_cast<MainMenuScreen*>(stack.top()), nullptr);
}

TEST(ScreenTransitionShould, incrementGameplayTickCountAfterReplace)
{
    ScreenStack stack;
    stack.requestPush(std::make_unique<MainMenuScreen>(stack));
    stack.applyCommands();

    stack.handleAction(Action::Confirm);
    stack.applyCommands();

    auto* const gameplay = dynamic_cast<GameplayScreen*>(stack.top());
    ASSERT_NE(gameplay, nullptr);
    EXPECT_EQ(gameplay->tickCount(), 0u);
    EXPECT_EQ(stack.size(), 1u);

    stack.update(sf::Time{});

    EXPECT_EQ(gameplay->tickCount(), 1u);
    EXPECT_EQ(stack.size(), 1u);
    EXPECT_EQ(dynamic_cast<MainMenuScreen*>(stack.top()), nullptr);
}

TEST(ScreenTransitionShould, highlightStartButtonOnHover)
{
    ScreenStack stack;
    auto menu = std::make_unique<MainMenuScreen>(stack);
    MainMenuScreen* const menuPtr = menu.get();
    stack.requestPush(std::move(menu));
    stack.applyCommands();

    const sf::Vector2i inside{
        static_cast<int>(MainMenuScreen::startButton.position.x + 10.f),
        static_cast<int>(MainMenuScreen::startButton.position.y + 10.f),
    };
    EXPECT_TRUE(stack.handleEvent(sf::Event{sf::Event::MouseMoved{.position = inside}}));
    EXPECT_EQ(menuPtr->hover(), MainMenuScreen::Hover::Start);
}

TEST(ScreenTransitionShould, replaceMenuWithGameplayOnStartClick)
{
    ScreenStack stack;
    stack.requestPush(std::make_unique<MainMenuScreen>(stack));
    stack.applyCommands();

    const sf::Vector2i inside{
        static_cast<int>(MainMenuScreen::startButton.position.x + 10.f),
        static_cast<int>(MainMenuScreen::startButton.position.y + 10.f),
    };
    EXPECT_TRUE(stack.handleEvent(
        sf::Event{sf::Event::MouseButtonPressed{.button = sf::Mouse::Button::Left, .position = inside}}));
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_NE(dynamic_cast<GameplayScreen*>(stack.top()), nullptr);
}

TEST(ScreenTransitionShould, requestCloseOnExitClick)
{
    ScreenStack stack;
    stack.requestPush(std::make_unique<MainMenuScreen>(stack));
    stack.applyCommands();

    const sf::Vector2i inside{
        static_cast<int>(MainMenuScreen::exitButton.position.x + 10.f),
        static_cast<int>(MainMenuScreen::exitButton.position.y + 10.f),
    };
    EXPECT_TRUE(stack.handleEvent(
        sf::Event{sf::Event::MouseButtonPressed{.button = sf::Mouse::Button::Left, .position = inside}}));

    EXPECT_TRUE(stack.closeRequested());
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(stack.top()), nullptr);
}

TEST(ScreenTransitionShould, keepMenuWhenEscapeIsPressed)
{
    ScreenStack stack;
    stack.requestPush(std::make_unique<MainMenuScreen>(stack));
    stack.applyCommands();

    EXPECT_FALSE(stack.handleAction(Action::Pause));
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(stack.top()), nullptr);
}
