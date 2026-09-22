#include <Game.hpp>
#include <mocks/time/ClockMock.hpp>
#include <mocks/window/WindowMock.hpp>
#include <time/FixedTimestep.hpp>
#include <unit_tests/fakes/ScreenSpy.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;

TEST(GameShould, closeWhenWindowReportsClosed)
{
    WindowMock window;
    NiceMock<ClockMock> clock;
    ScreenSpy screen;
    InSequence seq;

    EXPECT_CALL(window, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(sf::Event{sf::Event::Closed{}}));
    EXPECT_CALL(window, close());
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(std::optional<sf::Event>{}));
    EXPECT_CALL(window, clear());
    EXPECT_CALL(window, display());
    EXPECT_CALL(window, isOpen()).WillOnce(Return(false));

    Game{window, clock, screen}.run();
}

TEST(GameShould, updateOnceWhenClockReturnsOneTick)
{
    WindowMock window;
    ClockMock clock;
    ScreenSpy screen;
    InSequence seq;

    EXPECT_CALL(window, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(clock, restart()).WillOnce(Return(FixedTimestep::tick));
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(std::optional<sf::Event>{}));
    EXPECT_CALL(window, clear());
    EXPECT_CALL(window, display());
    EXPECT_CALL(window, isOpen()).WillOnce(Return(false));

    Game{window, clock, screen}.run();

    EXPECT_EQ(screen.updateCount, 1u);
    EXPECT_EQ(screen.drawCount, 1u);
    EXPECT_EQ(screen.lastDt, FixedTimestep::tick);
}
