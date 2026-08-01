/* Created by kblaszki */

#include <controllers/EventController.hpp>
#include <managers/KeyboardManager.hpp>
#include <managers/MouseManager.hpp>
#include <screens/GameScreen.hpp>

#include <mocks/controllers/ScreenControllerMock.hpp>
#include <mocks/window/EventCollectorMock.hpp>
#include <mocks/window/ScreenRendererMock.hpp>

#include <filesystem>
#include <gtest/gtest.h>
#include <resources/ResourceManager.hpp>

using namespace ::testing;

class GameScreenShould : public Test
{
protected:
    GameScreenShould()
        : resources{std::filesystem::path{RESOURCES_ROOT_FOR_TESTS}}
        , eventController{eventCollector}
    {
        eventController.emplace<KeyboardManager>();
        eventController.emplace<MouseManager>();
    }

    ResourceManager resources;
    StrictMock<EventCollectorMock> eventCollector{};
    EventController eventController;
    StrictMock<ScreenRendererMock> screenRenderer{};
    StrictMock<ScreenControllerMock> screenUpdater{};
};

TEST_F(GameScreenShould, drawPaddleWithoutClearingOrPresenting)
{
    GameScreen game{eventController, screenRenderer, screenUpdater, resources};

    EXPECT_CALL(screenRenderer, clear(_)).Times(0);
    EXPECT_CALL(screenRenderer, display()).Times(0);
    EXPECT_CALL(screenRenderer, draw(_, _)).Times(1);

    game.display();
}

TEST_F(GameScreenShould, updatePaddleWithDeltaTime)
{
    GameScreen game{eventController, screenRenderer, screenUpdater, resources};

    sf::Event::KeyPressed rightPressed{};
    rightPressed.code = sf::Keyboard::Key::Right;
    eventController.get<ManagerOf::Keyboard>().handleEvent(sf::Event{rightPressed});

    EXPECT_NO_THROW(game.update(1.f / 60.f));
}
