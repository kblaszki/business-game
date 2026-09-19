/* Created by kblaszki */

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <Action.hpp>
#include <FixedTimestep.hpp>
#include <GameplayScreen.hpp>
#include <LevelId.hpp>
#include <MainMenuScreen.hpp>
#include <PauseScreen.hpp>
#include <ScreenStack.hpp>
#include <gtest/gtest.h>
#include <memory>

namespace
{
class NullRenderTarget : public sf::RenderTarget
{
public:
    sf::Vector2u getSize() const override
    {
        return {1u, 1u};
    }
};

void simulateGameUpdate(ScreenStack& stack, FixedTimestep& timestep, sf::Clock& clock)
{
    if(stack.pauseIsTop())
    {
        clock.restart();
        return;
    }

    const auto ticks = timestep.drain(clock.restart());
    for(auto i = 0u; i < ticks; ++i)
    {
        stack.update(FixedTimestep::tick);
    }
}
} // namespace

TEST(PauseBlocksTicksShould, pushOnlyOnePauseOverlayEvenWhenRequestedTwice)
{
    ScreenStack stack;
    auto gameplay = std::make_unique<GameplayScreen>(stack, LevelId::Arkanoid);
    GameplayScreen* const gameplayPtr = gameplay.get();
    stack.requestPush(std::move(gameplay));
    stack.applyCommands();

    for(auto i = 0; i < 5; ++i)
    {
        stack.update(sf::Time{});
    }
    const auto ticksBeforePause = gameplayPtr->tickCount();
    EXPECT_EQ(ticksBeforePause, 5u);

    stack.requestPauseOverlay();
    stack.requestPauseOverlay();
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 2u);
    EXPECT_TRUE(stack.pauseIsTop());
    EXPECT_NE(dynamic_cast<PauseScreen*>(stack.top()), nullptr);
    EXPECT_EQ(gameplayPtr->tickCount(), ticksBeforePause);

    stack.requestPauseOverlay();
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 2u);
    EXPECT_TRUE(stack.pauseIsTop());
    EXPECT_EQ(gameplayPtr->tickCount(), ticksBeforePause);
}

TEST(PauseBlocksTicksShould, leaveGameplayTickCountUnchangedWhilePaused)
{
    ScreenStack stack;
    auto gameplay = std::make_unique<GameplayScreen>(stack, LevelId::Arkanoid);
    GameplayScreen* const gameplayPtr = gameplay.get();
    stack.requestPush(std::move(gameplay));
    stack.applyCommands();

    for(auto i = 0; i < 5; ++i)
    {
        stack.update(sf::Time{});
    }
    const auto ticksBeforePause = gameplayPtr->tickCount();

    stack.requestPauseOverlay();
    stack.applyCommands();
    ASSERT_TRUE(stack.pauseIsTop());

    for(auto i = 0; i < 12; ++i)
    {
        stack.update(sf::Time{});
    }
    EXPECT_EQ(gameplayPtr->tickCount(), ticksBeforePause);

    FixedTimestep timestep;
    sf::Clock clock;
    for(auto i = 0; i < 8; ++i)
    {
        simulateGameUpdate(stack, timestep, clock);
    }
    EXPECT_EQ(gameplayPtr->tickCount(), ticksBeforePause);
}

TEST(PauseBlocksTicksShould, resumeWithPauseAndKeepSameGameplayInstance)
{
    ScreenStack stack;
    auto gameplay = std::make_unique<GameplayScreen>(stack, LevelId::Arkanoid);
    GameplayScreen* const gameplayPtr = gameplay.get();
    stack.requestPush(std::move(gameplay));
    stack.applyCommands();

    for(auto i = 0; i < 3; ++i)
    {
        stack.update(sf::Time{});
    }
    const auto ticksBeforePause = gameplayPtr->tickCount();

    stack.requestPauseOverlay();
    stack.applyCommands();
    ASSERT_TRUE(stack.pauseIsTop());

    for(auto i = 0; i < 6; ++i)
    {
        stack.update(sf::Time{});
    }
    EXPECT_EQ(gameplayPtr->tickCount(), ticksBeforePause);

    EXPECT_TRUE(stack.handleAction(Action::Pause));
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_TRUE(stack.gameplayIsTop());
    EXPECT_EQ(stack.top(), gameplayPtr);
    EXPECT_EQ(gameplayPtr->tickCount(), ticksBeforePause);
}

TEST(PauseBlocksTicksShould, resumeWithCancelAndKeepTickCountFrozenAcrossPausedUpdates)
{
    ScreenStack stack;
    auto gameplay = std::make_unique<GameplayScreen>(stack, LevelId::Arkanoid);
    GameplayScreen* const gameplayPtr = gameplay.get();
    stack.requestPush(std::move(gameplay));
    stack.applyCommands();

    stack.update(sf::Time{});
    const auto ticksBeforePause = gameplayPtr->tickCount();

    stack.requestPauseOverlay();
    stack.applyCommands();

    for(auto i = 0; i < 4; ++i)
    {
        stack.update(sf::Time{});
    }
    EXPECT_EQ(gameplayPtr->tickCount(), ticksBeforePause);

    EXPECT_TRUE(stack.handleAction(Action::Cancel));
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_EQ(stack.top(), gameplayPtr);
    EXPECT_EQ(gameplayPtr->tickCount(), ticksBeforePause);
}

TEST(PauseBlocksTicksShould, quitToMenuOnConfirmAndDestroyGameplay)
{
    ScreenStack stack;
    auto gameplay = std::make_unique<GameplayScreen>(stack, LevelId::Arkanoid);
    stack.requestPush(std::move(gameplay));
    stack.applyCommands();

    stack.requestPauseOverlay();
    stack.applyCommands();
    ASSERT_TRUE(stack.pauseIsTop());

    EXPECT_TRUE(stack.handleAction(Action::Confirm));
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(stack.top()), nullptr);
    EXPECT_EQ(dynamic_cast<GameplayScreen*>(stack.top()), nullptr);
    EXPECT_FALSE(stack.gameplayIsTop());
    EXPECT_FALSE(stack.pauseIsTop());
}

TEST(PauseBlocksTicksShould, letGameplayConsumeFirstPauseThenOverlayConsumesLater)
{
    ScreenStack stack;
    auto gameplay = std::make_unique<GameplayScreen>(stack, LevelId::Arkanoid);
    GameplayScreen* const gameplayPtr = gameplay.get();
    stack.requestPush(std::move(gameplay));
    stack.applyCommands();

    EXPECT_TRUE(stack.handleAction(Action::Pause));
    stack.applyCommands();

    EXPECT_TRUE(stack.pauseIsTop());
    EXPECT_EQ(stack.size(), 2u);
    EXPECT_EQ(gameplayPtr->handleActionCount(), 1u);

    EXPECT_TRUE(stack.handleAction(Action::Pause));
    EXPECT_EQ(gameplayPtr->handleActionCount(), 1u);
    EXPECT_TRUE(stack.handleAction(Action::Cancel));
    EXPECT_EQ(gameplayPtr->handleActionCount(), 1u);
}

TEST(PauseBlocksTicksShould, ignorePauseOverlayRequestWhenMenuIsTop)
{
    ScreenStack stack;
    stack.requestPush(std::make_unique<MainMenuScreen>(stack));
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_FALSE(stack.gameplayIsTop());

    stack.requestPauseOverlay();
    stack.applyCommands();

    EXPECT_EQ(stack.size(), 1u);
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(stack.top()), nullptr);
    EXPECT_FALSE(stack.pauseIsTop());
}

TEST(PauseBlocksTicksShould, keepDrawingGameplayUnderPauseOverlay)
{
    ScreenStack stack;
    NullRenderTarget target;
    auto gameplay = std::make_unique<GameplayScreen>(stack, LevelId::Arkanoid);
    GameplayScreen* const gameplayPtr = gameplay.get();
    stack.requestPush(std::move(gameplay));
    stack.applyCommands();

    stack.draw(target);
    EXPECT_EQ(gameplayPtr->drawCount(), 1u);

    stack.requestPauseOverlay();
    stack.applyCommands();
    ASSERT_TRUE(stack.pauseIsTop());

    stack.draw(target);
    stack.draw(target);
    EXPECT_EQ(gameplayPtr->drawCount(), 3u);
}
