/* Created by kblaszki */

#include <controllers/EventController.hpp>

#include <mocks/core/EventCollectorMock.hpp>
#include <mocks/managers/ClosedEventManagerMock.hpp>
#include <mocks/managers/KeyPressedManagerMock.hpp>

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
    EXPECT_NO_THROW(eventController.emplace<ClosedEventManagerMock>());

    EXPECT_THROW(eventController.emplace<ClosedEventManagerMock>(), std::runtime_error);
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

    auto closedEventManagerMock = std::make_unique<ClosedEventManagerMock>();
    auto keyPressedManagerMock = std::make_unique<KeyPressedManagerMock>();

    EXPECT_CALL(*closedEventManagerMock, handleEvent(_)).Times(1);
    EXPECT_CALL(*keyPressedManagerMock, handleEvent(_)).Times(0);

    EXPECT_NO_THROW(eventController.emplace<ClosedEventManagerMock>(std::move(closedEventManagerMock)));
    EXPECT_NO_THROW(eventController.emplace<KeyPressedManagerMock>(std::move(keyPressedManagerMock)));

    eventController.handleEvents();
}