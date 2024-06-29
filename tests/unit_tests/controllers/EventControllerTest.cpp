/* Created by kblaszki */

#include <controllers/EventController.hpp>

#include <mocks/managers/GameExitManagerMock.hpp>
#include <mocks/managers/KeyboardManagerMock.hpp>
#include <mocks/window/EventCollectorMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class EventControllerShould : public Test
{
protected:
    StrictMock<EventCollectorMock> eventCollectorMock{};
    EventController eventController{eventCollectorMock};
};

TEST_F(EventControllerShould, throwRuntimeErrorWhenTrySecondTimeRegisterHandlerForTheSameEvent)
{
    EXPECT_NO_THROW(eventController.emplace<StrictMock<GameExitManagerMock>>());

    EXPECT_THROW(eventController.emplace<StrictMock<GameExitManagerMock>>(), std::runtime_error);
}

TEST_F(EventControllerShould, handleCorrectlyEvent)
{

    EXPECT_CALL(eventCollectorMock, pollEvent(_))
        .Times(2)
        .WillOnce(Invoke([](sf::Event& event) {
            event.type = sf::Event::Closed;
            return true;
        }))
        .WillOnce(Return(false));

    auto gameExitManagerMock = std::make_unique<StrictMock<GameExitManagerMock>>();
    auto keyboardManagerMock = std::make_unique<StrictMock<KeyboardManagerMock>>();

    EXPECT_CALL(*gameExitManagerMock, handleEvent(_)).Times(1);
    EXPECT_CALL(*keyboardManagerMock, handleEvent(_)).Times(0);

    EXPECT_NO_THROW(eventController.emplace(std::move(gameExitManagerMock)));
    EXPECT_NO_THROW(eventController.emplace(std::move(keyboardManagerMock)));

    eventController.handleEvents();
}