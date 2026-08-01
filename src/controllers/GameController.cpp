/* Created by kblaszki */

#include "GameController.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

namespace
{
constexpr float FIXED_DT = 1.f / 60.f;
constexpr float MAX_FRAME_TIME = 0.25f;

GameController::FrameTimeProvider makeDefaultFrameTimeProvider()
{
    auto clock = std::make_shared<sf::Clock>();
    return [clock]() { return clock->restart().asSeconds(); };
}
} // namespace

GameController::GameController(std::unique_ptr<WindowI>&& window,
                               std::unique_ptr<EventControllerI>&& eventController,
                               std::unique_ptr<ScreenControllerI>&& screenController,
                               FrameTimeProvider frameTimeProvider)
    : window{std::move(window)}
    , eventController{std::move(eventController)}
    , screenController{std::move(screenController)}
    , frameTimeProvider{frameTimeProvider ? std::move(frameTimeProvider) : makeDefaultFrameTimeProvider()}
{
}

void GameController::run()
{
    float accumulator = 0.f;

    while(window->isOpen())
    {
        float frameTime = frameTimeProvider();
        if(frameTime > MAX_FRAME_TIME)
        {
            frameTime = MAX_FRAME_TIME;
        }
        accumulator += frameTime;

        eventController->handleEvents();

        while(accumulator >= FIXED_DT)
        {
            screenController->update(FIXED_DT);
            accumulator -= FIXED_DT;
        }

        screenController->display();
    }
}
