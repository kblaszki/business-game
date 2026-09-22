#include <Game.hpp>
#include <mocks/window/WindowMock.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::InSequence;
using ::testing::Return;

TEST(GameShould, closeWhenWindowReportsClosed)
{
    WindowMock window;
    InSequence seq;

    EXPECT_CALL(window, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(sf::Event{sf::Event::Closed{}}));
    EXPECT_CALL(window, close());
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(std::optional<sf::Event>{}));
    EXPECT_CALL(window, clear());
    EXPECT_CALL(window, display());
    EXPECT_CALL(window, isOpen()).WillOnce(Return(false));

    Game{window}.run();
}
