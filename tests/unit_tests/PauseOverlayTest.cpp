#include <window/DrawerI.hpp>

#include <mocks/time/ClockMock.hpp>
#include <mocks/window/WindowMock.hpp>

#include <Game.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <input/InputMapper.hpp>
#include <memory>
#include <screen/GameplayScreen.hpp>
#include <screen/MainMenuScreen.hpp>
#include <screen/PauseScreen.hpp>
#include <screen/ScreenStack.hpp>
#include <time/FixedTimestep.hpp>

using ::testing::NiceMock;
using ::testing::Return;

namespace
{
class CountingDrawer : public DrawerI
{
public:
    void draw(const sf::Drawable&) override
    {
        ++drawCount;
    }

    std::uint32_t drawCount{};
};

GameplayScreen& pushPlay(ScreenStack& screens)
{
    auto play = std::make_unique<GameplayScreen>(screens);
    GameplayScreen& screen = *play;
    screens.push(std::move(play));
    return screen;
}

void apply(ScreenStack& screens)
{
    CountingDrawer drawer;
    screens.draw(drawer);
}

void runOneFrame(WindowMock& window, ClockMock& clock, ScreenStack& screens, const sf::Event& event)
{
    EXPECT_CALL(window, isOpen()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(event)).WillOnce(Return(std::optional<sf::Event>{}));
    EXPECT_CALL(window, clear());
    EXPECT_CALL(window, display());
    Game{window, clock, screens}.run();
}
} // namespace

TEST(PauseOverlayShould, pushOverlayOnPauseAction)
{
    ScreenStack screens;
    pushPlay(screens);

    EXPECT_TRUE(screens.handleAction(Action::Pause));
    apply(screens);

    EXPECT_EQ(screens.size(), 2u);
    EXPECT_NE(dynamic_cast<PauseScreen*>(screens.top()), nullptr);
}

TEST(PauseOverlayShould, freezeGameplayTicksAndStillDrawUnderneath)
{
    ScreenStack screens;
    GameplayScreen& play = pushPlay(screens);
    play.handleAction(Action::Confirm);
    play.update(FixedTimestep::tick);
    screens.handleAction(Action::Pause);
    apply(screens);

    const auto frozenTicks = play.tickCount();
    const auto frozenPose = play.world().ball().position();
    screens.update(FixedTimestep::tick);
    screens.update(FixedTimestep::tick);
    EXPECT_EQ(play.tickCount(), frozenTicks);
    EXPECT_EQ(play.world().ball().position(), frozenPose);

    CountingDrawer drawer;
    screens.draw(drawer);
    EXPECT_GT(drawer.drawCount, 5u);
}

TEST(PauseOverlayShould, resumeOnPauseOrCancelAndTickAgain)
{
    ScreenStack screens;
    GameplayScreen& play = pushPlay(screens);
    screens.handleAction(Action::Pause);
    apply(screens);

    EXPECT_TRUE(screens.handleAction(Action::Cancel));
    apply(screens);

    EXPECT_EQ(screens.size(), 1u);
    EXPECT_EQ(screens.top(), &play);
    EXPECT_FALSE(screens.closeRequested());

    play.handleAction(Action::Confirm);
    const auto before = play.world().ball().position();
    screens.update(FixedTimestep::tick);
    EXPECT_EQ(play.tickCount(), 1u);
    EXPECT_NE(play.world().ball().position(), before);

    screens.handleAction(Action::Pause);
    apply(screens);
    EXPECT_TRUE(screens.handleAction(Action::Pause));
    apply(screens);
    EXPECT_EQ(screens.size(), 1u);
    EXPECT_EQ(screens.top(), &play);
}

TEST(PauseOverlayShould, quitToMenuOnConfirm)
{
    ScreenStack screens;
    pushPlay(screens);
    screens.handleAction(Action::Pause);
    apply(screens);

    EXPECT_TRUE(screens.handleAction(Action::Confirm));
    apply(screens);

    EXPECT_EQ(screens.size(), 1u);
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(screens.top()), nullptr);
}

TEST(PauseOverlayShould, enqueueOnlyOneOverlayPerFrame)
{
    ScreenStack screens;
    pushPlay(screens);

    screens.handleAction(Action::Pause);
    screens.requestPauseOverlay();
    EXPECT_EQ(screens.size(), 1u);

    apply(screens);
    EXPECT_EQ(screens.size(), 2u);
}

TEST(PauseOverlayShould, openFromFocusLostOnGameplay)
{
    NiceMock<WindowMock> window;
    NiceMock<ClockMock> clock;
    ScreenStack screens;
    pushPlay(screens);

    runOneFrame(window, clock, screens, sf::Event{sf::Event::FocusLost{}});

    EXPECT_EQ(screens.size(), 2u);
    EXPECT_NE(dynamic_cast<PauseScreen*>(screens.top()), nullptr);
}

TEST(PauseOverlayShould, ignoreFocusLostOnMenu)
{
    NiceMock<WindowMock> window;
    NiceMock<ClockMock> clock;
    ScreenStack screens;
    screens.push(std::make_unique<MainMenuScreen>(screens));

    runOneFrame(window, clock, screens, sf::Event{sf::Event::FocusLost{}});

    EXPECT_EQ(screens.size(), 1u);
    EXPECT_NE(dynamic_cast<MainMenuScreen*>(screens.top()), nullptr);
}

TEST(PauseOverlayShould, ignoreFocusLostWhenAlreadyPaused)
{
    NiceMock<WindowMock> window;
    NiceMock<ClockMock> clock;
    ScreenStack screens;
    pushPlay(screens);
    screens.requestPauseOverlay();
    EXPECT_EQ(screens.size(), 2u);

    runOneFrame(window, clock, screens, sf::Event{sf::Event::FocusLost{}});

    EXPECT_EQ(screens.size(), 2u);
    EXPECT_NE(dynamic_cast<PauseScreen*>(screens.top()), nullptr);
}

TEST(PauseOverlayShould, notResumeOnFocusGained)
{
    NiceMock<WindowMock> window;
    NiceMock<ClockMock> clock;
    ScreenStack screens;
    pushPlay(screens);
    screens.requestPauseOverlay();

    runOneFrame(window, clock, screens, sf::Event{sf::Event::FocusGained{}});

    EXPECT_EQ(screens.size(), 2u);
    EXPECT_NE(dynamic_cast<PauseScreen*>(screens.top()), nullptr);
}

TEST(PauseOverlayShould, notMapFocusLostToPauseAction)
{
    const InputMapper mapper;
    EXPECT_EQ(mapper.mapEvent(sf::Event{sf::Event::FocusLost{}}), std::nullopt);
}
