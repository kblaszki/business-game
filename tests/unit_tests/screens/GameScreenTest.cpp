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

TEST_F(GameScreenShould, drawArenaWithoutClearingOrPresenting)
{
    GameScreen game{eventController, screenRenderer, screenUpdater, resources};

    EXPECT_CALL(screenRenderer, clear(_)).Times(0);
    EXPECT_CALL(screenRenderer, display()).Times(0);
    // 50 bricks + paddle + ball + HUD text
    EXPECT_CALL(screenRenderer, draw(_, _)).Times(53);

    game.display();
}

TEST_F(GameScreenShould, updateWithoutCrashing)
{
    GameScreen game{eventController, screenRenderer, screenUpdater, resources};

    sf::Event::KeyPressed rightPressed{};
    rightPressed.code = sf::Keyboard::Key::Right;
    eventController.get<ManagerOf::Keyboard>().handleEvent(sf::Event{rightPressed});

    EXPECT_NO_THROW(game.update(1.f / 60.f));
}

TEST_F(GameScreenShould, destroyBrickOnBallCollision)
{
    GameScreen game{eventController, screenRenderer, screenUpdater, resources};
    const std::size_t bricksBefore = game.remainingBricks();
    ASSERT_GT(bricksBefore, 0u);

    game.ball().setPosition({640.f, 80.f});
    game.ball().setVelocity({0.f, -200.f});
    game.update(1.f / 60.f);

    EXPECT_LT(game.remainingBricks(), bricksBefore);
}

TEST_F(GameScreenShould, loseLifeWhenBallFallsBelowArena)
{
    GameScreen game{eventController, screenRenderer, screenUpdater, resources};
    ASSERT_EQ(GameScreen::INITIAL_LIVES, game.lives());

    game.ball().setPosition({640.f, GameScreen::ARENA_HEIGHT + 20.f});
    game.ball().setVelocity({0.f, 100.f});
    game.update(1.f / 60.f);

    EXPECT_EQ(GameScreen::INITIAL_LIVES - 1, game.lives());
}

TEST_F(GameScreenShould, returnToMenuWhenLivesReachZero)
{
    GameScreen game{eventController, screenRenderer, screenUpdater, resources};

    EXPECT_CALL(screenUpdater, replaceScreen(_)).Times(1);

    for(int i = 0; i < GameScreen::INITIAL_LIVES; ++i)
    {
        game.ball().setPosition({640.f, GameScreen::ARENA_HEIGHT + 20.f});
        game.ball().setVelocity({0.f, 100.f});
        game.update(1.f / 60.f);
    }

    EXPECT_EQ(0, game.lives());
}

TEST_F(GameScreenShould, returnToMenuWhenAllBricksDestroyed)
{
    GameScreen game{eventController, screenRenderer, screenUpdater, resources};

    EXPECT_CALL(screenUpdater, replaceScreen(_)).Times(1);

    while(const auto brickBounds = game.firstAliveBrickBounds())
    {
        game.ball().setPosition(
            {brickBounds->position.x + brickBounds->size.x / 2.f, brickBounds->position.y + brickBounds->size.y / 2.f});
        game.ball().setVelocity({0.f, 0.f});
        game.update(1.f / 60.f);
    }

    EXPECT_EQ(0u, game.remainingBricks());
}
