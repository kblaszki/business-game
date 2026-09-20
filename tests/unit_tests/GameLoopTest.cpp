/* Created by kblaszki */

#include "fakes/WindowMock.hpp"

#include <Game.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::_;
using testing::AtLeast;
using testing::NiceMock;
using testing::Return;

TEST(GameShould, runOneFrameThroughWindowIWithoutConstructingRenderWindow)
{
    NiceMock<WindowMock> window;
    EXPECT_CALL(window, isOpen()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(window, pollEvent()).WillOnce(Return(std::nullopt));
    EXPECT_CALL(window, clear(_)).Times(1);
    EXPECT_CALL(window, draw(_, _)).Times(AtLeast(1));
    EXPECT_CALL(window, display()).Times(1);

    Game{}.run(window);
}
