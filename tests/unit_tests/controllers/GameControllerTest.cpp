/* Created by kblaszki */

#include <controllers/EventController.hpp>
#include <controllers/GameController.hpp>

#include <mocks/controllers/EventControllerMock.hpp>
#include <mocks/controllers/ScreenControllerMock.hpp>
#include <mocks/managers/GameExitManagerMock.hpp>
#include <mocks/managers/KeyboardManagerMock.hpp>
#include <mocks/window/WindowMock.hpp>

#include <gtest/gtest.h>

using namespace ::testing;

class GameControllerShould : public Test
{
public:
    GameControllerShould()
        : screenControllerMock{std::make_unique<StrictMock<ScreenControllerMock>>()}
        , gameExitManagerMock{std::make_unique<StrictMock<GameExitManagerMock>>()}
        , keyboardManagerMock{std::make_unique<StrictMock<KeyboardManagerMock>>()}
        , windowMock{std::make_unique<StrictMock<WindowMock>>()}
        , eventController{std::make_unique<EventController>(*windowMock)}
    {
    }

protected:
    std::unique_ptr<StrictMock<ScreenControllerMock>> screenControllerMock;
    std::unique_ptr<StrictMock<GameExitManagerMock>> gameExitManagerMock;
    std::unique_ptr<StrictMock<KeyboardManagerMock>> keyboardManagerMock;
    std::unique_ptr<StrictMock<WindowMock>> windowMock;
    std::unique_ptr<EventController> eventController;
};

TEST_F(GameControllerShould, doOneIterationCorrectly)
{
    EXPECT_CALL(*windowMock, isOpen()).Times(2).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*windowMock, pollEvent()).Times(1).WillOnce(Return(std::nullopt));
    EXPECT_CALL(*screenControllerMock, update()).Times(1);
    EXPECT_CALL(*screenControllerMock, display()).Times(1);

    GameController game{std::move(windowMock), std::move(eventController), std::move(screenControllerMock)};
    game.run();
}

TEST_F(GameControllerShould, closeWindowAfterClickingTheExitButton)
{
    EXPECT_CALL(*windowMock, isOpen()).Times(2).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*windowMock, pollEvent())
        .Times(2)
        .WillOnce(Return(sf::Event{sf::Event::Closed{}}))
        .WillOnce(Return(std::nullopt));
    EXPECT_CALL(*windowMock, close()).Times(1);
    EXPECT_CALL(*screenControllerMock, update()).Times(1);
    EXPECT_CALL(*screenControllerMock, display()).Times(1);

    EXPECT_CALL(*gameExitManagerMock, handleEvent(_))
        .Times(1)
        .WillOnce(Invoke([window = windowMock.get()](const sf::Event&) { window->close(); }));
    eventController->emplace(std::move(gameExitManagerMock));

    GameController game{std::move(windowMock), std::move(eventController), std::move(screenControllerMock)};
    game.run();
}

TEST_F(GameControllerShould, closeWindowAfterEscapeKeyPressed)
{
    EXPECT_CALL(*windowMock, isOpen()).Times(2).WillOnce(Return(true)).WillOnce(Return(false));
    sf::Event::KeyPressed escapePressed{};
    escapePressed.code = sf::Keyboard::Key::Escape;
    escapePressed.scancode = sf::Keyboard::Scancode::Escape;
    EXPECT_CALL(*windowMock, pollEvent())
        .Times(2)
        .WillOnce(Return(sf::Event{escapePressed}))
        .WillOnce(Return(std::nullopt));
    EXPECT_CALL(*windowMock, close()).Times(1);
    EXPECT_CALL(*screenControllerMock, update()).Times(1);
    EXPECT_CALL(*screenControllerMock, display()).Times(1);

    EXPECT_CALL(*keyboardManagerMock, handleEvent(_))
        .Times(1)
        .WillOnce(Invoke([window = windowMock.get()](const sf::Event& event) {
            const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
            if(keyPressed and sf::Keyboard::Key::Escape == keyPressed->code)
            {
                window->close();
            }
        }));
    eventController->emplace(std::move(keyboardManagerMock));

    GameController game{std::move(windowMock), std::move(eventController), std::move(screenControllerMock)};
    game.run();
}