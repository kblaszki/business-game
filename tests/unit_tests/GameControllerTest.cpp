/* Created by kblaszki */

#include <controllers/EventController.hpp>
#include <controllers/GameController.hpp>

#include <mocks/controllers/EventControllerMock.hpp>
#include <mocks/core/WindowMock.hpp>
#include <mocks/managers/EventManagerMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class GameControllerShould : public Test
{
public:
    GameControllerShould()
        : closedEventManagerMock{std::make_unique<
              StrictMock<EventManagerMock<sf::Event::Closed>>>()}
        , keyPressedManagerMock{std::make_unique<
              StrictMock<EventManagerMock<sf::Event::KeyPressed>>>()}
        , windowMock{std::make_unique<StrictMock<WindowMock>>()}
        , eventController{std::make_unique<EventController>(*windowMock)}
    {
    }

protected:
    std::unique_ptr<StrictMock<EventManagerMock<sf::Event::Closed>>>
        closedEventManagerMock;
    std::unique_ptr<StrictMock<EventManagerMock<sf::Event::KeyPressed>>>
        keyPressedManagerMock;
    std::unique_ptr<StrictMock<WindowMock>> windowMock;
    std::unique_ptr<EventController> eventController;
};

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

    EXPECT_CALL(*closedEventManagerMock, handleEvent(_))
        .Times(1)
        .WillOnce(Invoke([window = windowMock.get()](const sf::Event&) {
            window->close();
        }));
    eventController->emplace<StrictMock<EventManagerMock<sf::Event::Closed>>>(
        std::move(closedEventManagerMock));

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
            event.key.scancode = sf::Keyboard::Scancode::Escape;
            return true;
        }))
        .WillOnce(Return(false));
    EXPECT_CALL(*windowMock, close()).Times(1);
    EXPECT_CALL(*windowMock, clear(_)).Times(1);
    EXPECT_CALL(*windowMock, draw(_, _)).Times(1);
    EXPECT_CALL(*windowMock, display()).Times(1);

    EXPECT_CALL(*keyPressedManagerMock, handleEvent(_))
        .Times(1)
        .WillOnce(Invoke([window = windowMock.get()](const sf::Event& event) {
            if(sf::Keyboard::Escape == event.key.code)
            {
                window->close();
            }
        }));
    eventController
        ->emplace<StrictMock<EventManagerMock<sf::Event::KeyPressed>>>(
            std::move(keyPressedManagerMock));

    GameController game{std::move(windowMock), std::move(eventController)};
    game.run();
}