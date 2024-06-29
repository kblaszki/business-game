/* Created by kblaszki */

#include <managers/GameExitManager.hpp>

#include <mocks/utils/ActionMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class GameExitManagerShould : public Test
{
protected:
    StrictMock<ActionMock> actionMock{};
    GameExitManager gameExitManager{};
};

TEST_F(GameExitManagerShould, properlyRegisterClosedEventHandler)
{
    EXPECT_NO_THROW(gameExitManager.registerExitHandler([this]() { actionMock.doAction(); }));
}

TEST_F(GameExitManagerShould, properlyHandleClosedEvent)
{
    EXPECT_CALL(actionMock, doAction()).Times(1);

    EXPECT_NO_THROW(gameExitManager.registerExitHandler([this]() { actionMock.doAction(); }));

    sf::Event closedEvent{};
    closedEvent.type = sf::Event::Closed;
    gameExitManager.handleEvent(closedEvent);
}