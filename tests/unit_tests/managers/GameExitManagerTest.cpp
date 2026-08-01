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
    GameExitManagerI::ExitUnRegisterer registration;
    EXPECT_NO_THROW(registration = gameExitManager.registerExitHandler([this]() { actionMock.doAction(); }));
}

TEST_F(GameExitManagerShould, properlyHandleClosedEvent)
{
    EXPECT_CALL(windowMock, close()).Times(1);
    EXPECT_CALL(actionMock, doAction()).Times(1);

    auto registration = gameExitManager.registerExitHandler([this]() { actionMock.doAction(); });

    gameExitManager.handleEvent(sf::Event{sf::Event::Closed{}});
}
