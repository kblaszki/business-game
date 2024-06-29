/* Created by kblaszki */

#include <managers/MouseManager.hpp>

#include <mocks/utils/ActionMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class MouseManagerShould : public Test
{
protected:
    StrictMock<ActionMock> actionMock{};
    MouseManager mouseManager{};
};

TEST_F(MouseManagerShould, properlyRegisterMouseMovedEventHandler)
{
    EXPECT_NO_THROW(mouseManager.registerMoveHandler([this](const auto&) { actionMock.doAction(); }));
}

TEST_F(MouseManagerShould, properlyHandleMouseMovedEvent)
{
    EXPECT_CALL(actionMock, doAction()).Times(1);

    EXPECT_NO_THROW(mouseManager.registerMoveHandler([this](const auto&) { actionMock.doAction(); }));

    sf::Event mouseMovedEvent{};
    mouseMovedEvent.type = sf::Event::MouseMoved;
    mouseManager.handleEvent(mouseMovedEvent);
}