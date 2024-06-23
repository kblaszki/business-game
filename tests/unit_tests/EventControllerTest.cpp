/* Created by kblaszki */

#include <controllers/EventController.hpp>

#include <mocks/core/EventCollectorMock.hpp>
#include <mocks/managers/EventManagerMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class EventControllerShould : public Test
{
protected:
    StrictMock<EventCollectorMock> eventCollectorMock{};
    EventController eventController{eventCollectorMock};
};

TEST_F(EventControllerShould,
       throwRuntimeErrorWhenTrySecondTimeRegisterHandlerForTheSameEvent)
{
    EXPECT_NO_THROW(
        eventController.emplace<EventManagerMock<sf::Event::Closed>>());

    EXPECT_THROW(eventController.emplace<EventManagerMock<sf::Event::Closed>>(),
                 std::runtime_error);
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

    std::unique_ptr<EventManagerMock<sf::Event::Closed>>
        closedEventManagerMock =
            std::make_unique<EventManagerMock<sf::Event::Closed>>();
    std::unique_ptr<EventManagerMock<sf::Event::KeyPressed>>
        keyPressedEventManagerMock =
            std::make_unique<EventManagerMock<sf::Event::KeyPressed>>();

    EXPECT_CALL(*closedEventManagerMock, handleEvent(_)).Times(1);
    EXPECT_CALL(*keyPressedEventManagerMock, handleEvent(_)).Times(0);

    EXPECT_NO_THROW(
        eventController.emplace<EventManagerMock<sf::Event::Closed>>(
            std::move(closedEventManagerMock)));
    EXPECT_NO_THROW(
        eventController.emplace<EventManagerMock<sf::Event::KeyPressed>>(
            std::move(keyPressedEventManagerMock)));

    eventController.handleEvents();
}