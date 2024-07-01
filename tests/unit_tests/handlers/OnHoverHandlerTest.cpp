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
    MouseManager mouseManager{};
};

TEST_F(OnHoverHandlerShould, beProperlyConstructedAndDestructed)
{
    StrictMock<MouseManagerMock> mouseManagerMock{};
    StrictMock<ActionMock> actionMock{};

    EXPECT_CALL(actionMock, doAction()).Times(1);
    EXPECT_CALL(mouseManagerMock, registerMoveHandler(_)).Times(1).WillOnce(Return([&] { actionMock.doAction(); }));

    {
        OnHoverHandlerMock onHoverHandler{mouseManagerMock};
    }
}
TEST_F(OnHoverHandlerShould, invokeOnHoverWhenMouseEnters)
{
    StrictMock<OnHoverHandlerMock> handler{mouseManager};
    EXPECT_CALL(handler, isHover(100, 200)).Times(1).WillOnce(Return(true));
    EXPECT_CALL(handler, onHover()).Times(1);

    sf::Event event;
    event.type = sf::Event::MouseMoved;
    event.mouseMove.x = 100;
    event.mouseMove.y = 200;
    mouseManager.handleEvent(event);
}

TEST_F(OnHoverHandlerShould, invokeOnHoverOutWhenMouseLeaves)
{
    StrictMock<OnHoverHandlerMock> handler{mouseManager};
    EXPECT_CALL(handler, isHover(100, 200)).Times(2).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(handler, onHover()).Times(1);
    EXPECT_CALL(handler, onHoverOut()).Times(1);

    sf::Event event;
    event.type = sf::Event::MouseMoved;
    event.mouseMove.x = 100;
    event.mouseMove.y = 200;
    mouseManager.handleEvent(event);
    mouseManager.handleEvent(event);
}

TEST_F(OnHoverHandlerShould, notInvokeOnHoverWhenAlreadyHovered)
{
    StrictMock<OnHoverHandlerMock> handler{mouseManager};
    EXPECT_CALL(handler, isHover(100, 200)).Times(2).WillOnce(Return(true)).WillOnce(Return(true));
    EXPECT_CALL(handler, onHover()).Times(1);

    sf::Event event;
    event.type = sf::Event::MouseMoved;
    event.mouseMove.x = 100;
    event.mouseMove.y = 200;
    mouseManager.handleEvent(event);
    mouseManager.handleEvent(event);
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

    sf::Event event;
    event.type = sf::Event::MouseMoved;
    event.mouseMove.x = 100;
    event.mouseMove.y = 200;
    mouseManager.handleEvent(event);
    mouseManager.handleEvent(event);
    mouseManager.handleEvent(event);
}