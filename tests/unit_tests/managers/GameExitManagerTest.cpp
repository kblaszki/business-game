/* Created by kblaszki */

#include <managers/GameExitManager.hpp>

#include <mocks/utils/ActionMock.hpp>
#include <mocks/window/WindowMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class GameExitManagerShould : public Test
{
protected:
    StrictMock<WindowMock> windowMock{};
    StrictMock<ActionMock> actionMock{};
    GameExitManager gameExitManager{windowMock};
};

TEST_F(GameExitManagerShould, properlyRegisterClosedEventHandler)
{
    EXPECT_NO_THROW(gameExitManager.registerExitHandler([this]() { actionMock.doAction(); }));
}

TEST_F(GameExitManagerShould, properlyHandleClosedEvent)
{
    EXPECT_CALL(windowMock, close()).Times(1);
    EXPECT_CALL(actionMock, doAction()).Times(1);

    EXPECT_NO_THROW(gameExitManager.registerExitHandler([this]() { actionMock.doAction(); }));

    gameExitManager.handleEvent(sf::Event{sf::Event::Closed{}});
}