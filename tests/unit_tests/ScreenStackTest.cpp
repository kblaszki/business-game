/* Created by kblaszki */

#include "fakes/NullDrawer.hpp"
#include "fakes/SpyScreen.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <ScreenStack.hpp>
#include <gtest/gtest.h>
#include <memory>

TEST(ScreenStackShould, keepSizeUnchangedUntilApplyCommandsAfterTwoPushes)
{
    ScreenStack stack;
    stack.requestPush(std::make_unique<SpyScreen>());
    stack.requestPush(std::make_unique<SpyScreen>());

    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0u);

    stack.applyCommands();

    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.size(), 2u);
    EXPECT_NE(stack.top(), nullptr);
}

TEST(ScreenStackShould, destroyPoppedScreenOnlyAfterApplyCommands)
{
    ScreenStack stack;
    std::uint32_t destroyed{0};

    auto lower = std::make_unique<SpyScreen>();
    auto upper = std::make_unique<SpyScreen>();
    SpyScreen* const upperPtr = upper.get();
    upperPtr->destructorCount = &destroyed;
    upperPtr->onUpdate = [&stack] { stack.requestPop(); };

    stack.requestPush(std::move(lower));
    stack.requestPush(std::move(upper));
    stack.applyCommands();

    EXPECT_EQ(destroyed, 0u);

    stack.update(sf::Time{});

    EXPECT_TRUE(upperPtr->finishedUpdate);
    EXPECT_EQ(destroyed, 0u);
    EXPECT_EQ(stack.size(), 2u);

    stack.applyCommands();

    EXPECT_EQ(destroyed, 1u);
    EXPECT_EQ(stack.size(), 1u);
}

TEST(ScreenStackShould, skipLowerUpdateWhenTopBlocksUpdate)
{
    ScreenStack stack;
    auto lower = std::make_unique<SpyScreen>();
    auto upper = std::make_unique<SpyScreen>(true, false);
    SpyScreen* const lowerPtr = lower.get();
    SpyScreen* const upperPtr = upper.get();

    stack.requestPush(std::move(lower));
    stack.requestPush(std::move(upper));
    stack.applyCommands();

    stack.update(sf::Time{});

    EXPECT_EQ(upperPtr->updateCount, 1u);
    EXPECT_EQ(lowerPtr->updateCount, 0u);
}

TEST(ScreenStackShould, drawLowerWhenTopDoesNotBlockDraw)
{
    ScreenStack stack;
    auto lower = std::make_unique<SpyScreen>();
    auto upper = std::make_unique<SpyScreen>(false, false);
    SpyScreen* const lowerPtr = lower.get();
    SpyScreen* const upperPtr = upper.get();

    stack.requestPush(std::move(lower));
    stack.requestPush(std::move(upper));
    stack.applyCommands();

    EXPECT_EQ(stack.drawStartIndex(), 0u);

    NullDrawer drawer;
    stack.draw(drawer);
    EXPECT_EQ(lowerPtr->drawCount, 1u);
    EXPECT_EQ(upperPtr->drawCount, 1u);
}

TEST(ScreenStackShould, skipLowerDrawWhenTopBlocksDraw)
{
    ScreenStack stack;
    auto lower = std::make_unique<SpyScreen>();
    auto upper = std::make_unique<SpyScreen>(false, true);
    SpyScreen* const lowerPtr = lower.get();
    SpyScreen* const upperPtr = upper.get();

    stack.requestPush(std::move(lower));
    stack.requestPush(std::move(upper));
    stack.applyCommands();

    EXPECT_EQ(stack.drawStartIndex(), 1u);

    NullDrawer drawer;
    stack.draw(drawer);
    EXPECT_EQ(lowerPtr->drawCount, 0u);
    EXPECT_EQ(upperPtr->drawCount, 1u);
}

TEST(ScreenStackShould, stopHandleEventWhenTopConsumes)
{
    ScreenStack stack;
    auto lower = std::make_unique<SpyScreen>();
    auto upper = std::make_unique<SpyScreen>();
    SpyScreen* const lowerPtr = lower.get();
    SpyScreen* const upperPtr = upper.get();
    upperPtr->consumeEvent = true;

    stack.requestPush(std::move(lower));
    stack.requestPush(std::move(upper));
    stack.applyCommands();

    const sf::Event event{sf::Event::Closed{}};
    EXPECT_TRUE(stack.handleEvent(event));
    EXPECT_EQ(upperPtr->handleEventCount, 1u);
    EXPECT_EQ(lowerPtr->handleEventCount, 0u);
}

TEST(ScreenStackShould, treatEmptyStackOperationsAsNoOps)
{
    ScreenStack stack;
    const sf::Event event{sf::Event::Closed{}};

    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0u);
    EXPECT_EQ(stack.top(), nullptr);
    EXPECT_FALSE(stack.closeRequested());
    EXPECT_FALSE(stack.handleEvent(event));

    stack.update(sf::Time{});
    EXPECT_EQ(stack.drawStartIndex(), 0u);
    stack.applyCommands();
    stack.requestPop();
    stack.applyCommands();

    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0u);
    EXPECT_EQ(stack.top(), nullptr);
}

TEST(ScreenStackShould, setCloseRequestedFlagImmediately)
{
    ScreenStack stack;

    EXPECT_FALSE(stack.closeRequested());
    stack.requestClose();
    EXPECT_TRUE(stack.closeRequested());
}

TEST(ScreenStackShould, replaceTopAfterApplyCommands)
{
    ScreenStack stack;
    std::uint32_t destroyed{0};

    auto first = std::make_unique<SpyScreen>();
    first->destructorCount = &destroyed;
    stack.requestPush(std::move(first));
    stack.applyCommands();

    auto replacement = std::make_unique<SpyScreen>();
    SpyScreen* const replacementPtr = replacement.get();
    stack.requestReplace(std::move(replacement));

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_EQ(destroyed, 0u);

    stack.applyCommands();

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_EQ(destroyed, 1u);
    EXPECT_EQ(stack.top(), replacementPtr);
}
