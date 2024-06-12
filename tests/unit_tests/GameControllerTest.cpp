/* Created by kblaszki */

#include <controllers/EventController.hpp>
#include <controllers/GameController.hpp>

#include <mocks/controllers/EventControllerMock.hpp>
#include <mocks/core/WindowMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class GameControllerShould : public Test
{
public:
    GameControllerShould()
        : windowMock{std::make_unique<StrictMock<WindowMock>>()}
        , eventController{std::make_unique<EventController>(*windowMock)}
    {
    }

protected:
    std::unique_ptr<StrictMock<WindowMock>> windowMock;
    std::unique_ptr<EventControllerI> eventController;
};

TEST_F(GameControllerShould, properlyRegisterEventHandlers)
{
    auto eventControllerMock{
        std::make_unique<StrictMock<EventControllerMock>>()};

    EXPECT_CALL(*eventControllerMock,
                registerEventHandler(sf::Event::Closed, _))
        .Times(1);
    EXPECT_CALL(*eventControllerMock,
                registerEventHandler(sf::Event::KeyPressed, _))
        .Times(1);

    GameController game{std::move(windowMock), std::move(eventControllerMock)};
}

TEST_F(GameControllerShould, doOneIterationCorrectly)
{
    EXPECT_CALL(*windowMock, isOpen())
        .Times(2)
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(*windowMock, pollEvent(_)).Times(1).WillOnce(Return(false));
    EXPECT_CALL(*windowMock, clear(_)).Times(1);
    EXPECT_CALL(*windowMock, draw(_, _)).Times(1);
    EXPECT_CALL(*windowMock, display()).Times(1);

    GameController game{std::move(windowMock), std::move(eventController)};
    game.run();
}

TEST_F(GameControllerShould, closeWindowAfterClickingTheExitButton)
{
    EXPECT_CALL(*windowMock, isOpen())
        .Times(2)
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(*windowMock, pollEvent(_))
        .Times(2)
        .WillOnce(Invoke([](sf::Event& event) {
            event.type = sf::Event::Closed;
            return true;
        }))
        .WillOnce(Return(false));
    EXPECT_CALL(*windowMock, close()).Times(1);
    EXPECT_CALL(*windowMock, clear(_)).Times(1);
    EXPECT_CALL(*windowMock, draw(_, _)).Times(1);
    EXPECT_CALL(*windowMock, display()).Times(1);

    GameController game{std::move(windowMock), std::move(eventController)};
    game.run();
}

TEST_F(GameControllerShould, closeWindowAfterEscapeKeyPressed)
{
    EXPECT_CALL(*windowMock, isOpen())
        .Times(2)
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(*windowMock, pollEvent(_))
        .Times(2)
        .WillOnce(Invoke([](sf::Event& event) {
            event.type = sf::Event::KeyPressed;
            event.key.code = sf::Keyboard::Escape;
            return true;
        }))
        .WillOnce(Return(false));
    EXPECT_CALL(*windowMock, close()).Times(1);
    EXPECT_CALL(*windowMock, clear(_)).Times(1);
    EXPECT_CALL(*windowMock, draw(_, _)).Times(1);
    EXPECT_CALL(*windowMock, display()).Times(1);

    GameController game{std::move(windowMock), std::move(eventController)};
    game.run();
}