#include <SFML/Window/Keyboard.hpp>

#include <mocks/time/ClockMock.hpp>
#include <mocks/window/WindowMock.hpp>
#include <unit_tests/fakes/ScreenSpy.hpp>

#include <Game.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <screen/MainMenuScreen.hpp>
#include <screen/ScreenStack.hpp>
#include <time/FixedTimestep.hpp>

using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;

namespace
{
ScreenSpy& pushSpy(ScreenStack& screens)
{
    auto spy = std::make_unique<ScreenSpy>();
    ScreenSpy& screen = *spy;
    screens.push(std::move(spy));
    return screen;
}
} // namespace

TEST(GameShould, closeWhenWindowReportsClosed)
{
    WindowMock window;
    NiceMock<ClockMock> clock;
    ScreenStack screens;
    ScreenSpy& screen = pushSpy(screens);
    InSequence seq;

    EXPECT_CALL(window, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(sf::Event{sf::Event::Closed{}}));
    EXPECT_CALL(window, close());
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(std::optional<sf::Event>{}));
    EXPECT_CALL(window, clear());
    EXPECT_CALL(window, display());
    EXPECT_CALL(window, isOpen()).WillOnce(Return(false));

    Game{window, clock, screens}.run();

    EXPECT_EQ(screen.handleEventCount, 0u);
    EXPECT_EQ(screen.handleActionCount, 0u);
}

TEST(GameShould, closeWhenMenuCancelIsMapped)
{
    WindowMock window;
    NiceMock<ClockMock> clock;
    ScreenStack screens;
    screens.push(std::make_unique<MainMenuScreen>(screens));
    InSequence seq;

    sf::Event::KeyPressed pressed{};
    pressed.code = sf::Keyboard::Key::Backspace;

    EXPECT_CALL(window, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(sf::Event{pressed}));
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(std::optional<sf::Event>{}));
    EXPECT_CALL(window, close());
    EXPECT_CALL(window, clear());
    EXPECT_CALL(window, display());
    EXPECT_CALL(window, isOpen()).WillOnce(Return(false));

    Game{window, clock, screens}.run();

    EXPECT_TRUE(screens.closeRequested());
}

TEST(GameShould, routeEnterToHandleActionNotHandleEvent)
{
    WindowMock window;
    NiceMock<ClockMock> clock;
    ScreenStack screens;
    ScreenSpy& screen = pushSpy(screens);
    InSequence seq;

    sf::Event::KeyPressed pressed{};
    pressed.code = sf::Keyboard::Key::Enter;

    EXPECT_CALL(window, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(sf::Event{pressed}));
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(std::optional<sf::Event>{}));
    EXPECT_CALL(window, clear());
    EXPECT_CALL(window, display());
    EXPECT_CALL(window, isOpen()).WillOnce(Return(false));

    Game{window, clock, screens}.run();

    EXPECT_EQ(screen.handleActionCount, 1u);
    EXPECT_EQ(screen.lastAction, Action::Confirm);
    EXPECT_EQ(screen.handleEventCount, 0u);
}

TEST(GameShould, updateOnceWhenClockReturnsOneTick)
{
    WindowMock window;
    ClockMock clock;
    ScreenStack screens;
    ScreenSpy& screen = pushSpy(screens);
    InSequence seq;

    EXPECT_CALL(window, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(clock, restart()).WillOnce(Return(FixedTimestep::tick));
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(std::optional<sf::Event>{}));
    EXPECT_CALL(window, clear());
    EXPECT_CALL(window, display());
    EXPECT_CALL(window, isOpen()).WillOnce(Return(false));

    Game{window, clock, screens}.run();

    EXPECT_EQ(screen.updateCount, 1u);
    EXPECT_EQ(screen.drawCount, 1u);
    EXPECT_EQ(screen.lastDt, FixedTimestep::tick);
}
