/* Created by kblaszki */

#include <managers/MouseManager.hpp>

#include <mocks/handlers/OnHoverHandlerMock.hpp>
#include <mocks/managers/MouseManagerMock.hpp>
#include <mocks/utils/ActionMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class OnHoverHandlerShould : public Test
{
protected:
    sf::Event makeMouseMovedEvent(int x, int y) const
    {
        sf::Event::MouseMoved event{};
        event.position = {x, y};
        return sf::Event{event};
    }

    MouseManager mouseManager{};
};

TEST_F(OnHoverHandlerShould, beProperlyConstructedAndDestructed)
{
    StrictMock<MouseManagerMock> mouseManagerMock{};
    StrictMock<ActionMock> actionMock{};

    EXPECT_CALL(actionMock, doAction()).Times(1);
    EXPECT_CALL(mouseManagerMock, registerMoveHandler(_)).Times(1).WillOnce(Invoke([&](MouseManagerI::MoveHandler&&) {
        return MouseManagerI::MoveUnRegisterer([&] { actionMock.doAction(); });
    }));

    {
        OnHoverHandlerMock onHoverHandler{mouseManagerMock};
    }
}
TEST_F(OnHoverHandlerShould, invokeOnHoverWhenMouseEnters)
{
    StrictMock<OnHoverHandlerMock> handler{mouseManager};
    EXPECT_CALL(handler, isHover(100, 200)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(handler, onHover()).Times(1);

    mouseManager.handleEvent(makeMouseMovedEvent(100, 200));
}

TEST_F(OnHoverHandlerShould, invokeOnHoverOutWhenMouseLeaves)
{
    StrictMock<OnHoverHandlerMock> handler{mouseManager};
    EXPECT_CALL(handler, isHover(100, 200)).Times(2).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(handler, onHover()).Times(1);
    EXPECT_CALL(handler, onHoverOut()).Times(1);

    mouseManager.handleEvent(makeMouseMovedEvent(100, 200));
    mouseManager.handleEvent(makeMouseMovedEvent(100, 200));
}

TEST_F(OnHoverHandlerShould, notInvokeOnHoverWhenAlreadyHovered)
{
    StrictMock<OnHoverHandlerMock> handler{mouseManager};
    EXPECT_CALL(handler, isHover(100, 200)).Times(2).WillOnce(Return(true)).WillOnce(Return(true));
    EXPECT_CALL(handler, onHover()).Times(1);

    mouseManager.handleEvent(makeMouseMovedEvent(100, 200));
    mouseManager.handleEvent(makeMouseMovedEvent(100, 200));
}

TEST_F(OnHoverHandlerShould, notInvokeOnHoverOutWhenNotHovered)
{
    StrictMock<OnHoverHandlerMock> handler{mouseManager};
    EXPECT_CALL(handler, isHover(100, 200))
        .Times(3)
        .WillOnce(Return(true))
        .WillOnce(Return(false))
        .WillOnce(Return(false));
    EXPECT_CALL(handler, onHover()).Times(1);
    EXPECT_CALL(handler, onHoverOut()).Times(1);

    mouseManager.handleEvent(makeMouseMovedEvent(100, 200));
    mouseManager.handleEvent(makeMouseMovedEvent(100, 200));
    mouseManager.handleEvent(makeMouseMovedEvent(100, 200));
}
