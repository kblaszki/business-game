/* Created by kblaszki */

#include <managers/ClosedEventManager.hpp>

#include <mocks/utils/ActionMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class ClosedEventManagerShould : public Test
{
protected:
    StrictMock<ActionMock> actionMock{};
    ClosedEventManager closedEventManager{};
};

TEST_F(ClosedEventManagerShould, properlyRegisterClosedEventHandler)
{
    EXPECT_NO_THROW(closedEventManager.registerClosedHandler(
        [this]() { actionMock.doAction(); }));
}

TEST_F(ClosedEventManagerShould, properlyHandleClosedEvent)
{
    EXPECT_CALL(actionMock, doAction()).Times(1);

    EXPECT_NO_THROW(closedEventManager.registerClosedHandler(
        [this]() { actionMock.doAction(); }));

    sf::Event closedEvent{};
    closedEvent.type = sf::Event::Closed;
    closedEventManager.handleEvent(closedEvent);
}