/* Created by kblaszki */

#include <controllers/EventController.hpp>

#include <mocks/core/EventCollectorMock.hpp>
#include <mocks/utils/ActionMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class EventControllerShould : public Test
{
protected:
    StrictMock<EventCollectorMock> eventCollectorMock{};
    StrictMock<ActionMock> closeActionMock{};
    EventController eventController{eventCollectorMock};
};

TEST_F(EventControllerShould, properlyRegisterEventHandler)
{
    EXPECT_NO_THROW(eventController.registerEventHandler(
        sf::Event::Closed,
        [this](const sf::Event&) { closeActionMock.doAction(); }));
}

TEST_F(EventControllerShould,
       throwRuntimeErrorWhenTrySecondTimeRegisterHandlerForTheSameEvent)
{
    EXPECT_NO_THROW(eventController.registerEventHandler(
        sf::Event::Closed,
        [this](const sf::Event&) { closeActionMock.doAction(); }));

    EXPECT_THROW(eventController.registerEventHandler(
                     sf::Event::Closed,
                     [this](const sf::Event&) { closeActionMock.doAction(); }),
                 std::runtime_error);
}

TEST_F(EventControllerShould,
       throwExceptionWhenTryUnregisterNotRegisteredEventHandler)
{
    EXPECT_THROW(eventController.unregisterEventHandler(sf::Event::Closed),
                 std::runtime_error);
}

TEST_F(EventControllerShould, properlyUnregisterEventHandler)
{
    EXPECT_NO_THROW(eventController.registerEventHandler(
        sf::Event::Closed,
        [this](const sf::Event&) { closeActionMock.doAction(); }));

    EXPECT_NO_THROW(eventController.unregisterEventHandler(sf::Event::Closed));

    EXPECT_NO_THROW(eventController.registerEventHandler(
        sf::Event::Closed,
        [this](const sf::Event&) { closeActionMock.doAction(); }));
}

TEST_F(EventControllerShould, handleCorrectlyEvent)
{
    StrictMock<ActionMock> keyPressedActionMock{};

    EXPECT_CALL(eventCollectorMock, pollEvent(_))
        .Times(2)
        .WillOnce(Invoke([](sf::Event& event) {
            event.type = sf::Event::Closed;
            return true;
        }))
        .WillOnce(Return(false));
    EXPECT_CALL(closeActionMock, doAction()).Times(1);
    EXPECT_CALL(keyPressedActionMock, doAction()).Times(0);

    EXPECT_NO_THROW(eventController.registerEventHandler(
        sf::Event::Closed,
        [this](const sf::Event&) { closeActionMock.doAction(); }));
    EXPECT_NO_THROW(eventController.registerEventHandler(
        sf::Event::KeyPressed,
        [&](const sf::Event&) { keyPressedActionMock.doAction(); }));

    eventController.handleEvents();
}