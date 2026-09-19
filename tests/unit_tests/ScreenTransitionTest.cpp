/* Created by kblaszki */

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
