/* Created by kblaszki */

#include <managers/MouseMovedManager.hpp>

#include <mocks/utils/ActionMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class MouseMovedManagerShould : public Test
{
protected:
    StrictMock<ActionMock> actionMock{};
    MouseMovedManager mouseMovedManager{};
};

TEST_F(MouseMovedManagerShould, properlyRegisterMouseMovedEventHandler)
{
    EXPECT_NO_THROW(mouseMovedManager.registerHandler([this](const auto&) { actionMock.doAction(); }));
}

TEST_F(MouseMovedManagerShould, properlyHandleMouseMovedEvent)
{
    EXPECT_CALL(actionMock, doAction()).Times(1);

    EXPECT_NO_THROW(mouseMovedManager.registerHandler([this](const auto&) { actionMock.doAction(); }));

    sf::Event mouseMovedEvent{};
    mouseMovedEvent.type = sf::Event::MouseMoved;
    mouseMovedManager.handleEvent(mouseMovedEvent);
}