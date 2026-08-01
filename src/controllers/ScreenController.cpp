/* Created by kblaszki */

#include "ScreenController.hpp"

#include <stdexcept>

ScreenController::ScreenController(ScreenRendererI& screenRenderer, ScreenFactory makeInitialScreen)
    : screenRenderer{screenRenderer}
{
    auto initialScreen = makeInitialScreen(*this);
    if(not initialScreen)
    {
        throw std::runtime_error("ScreenController: initial screen factory returned null");
    }
    screenStack.push_back(std::move(initialScreen));
}

void ScreenController::pushScreen(std::unique_ptr<ScreenI>&& screen)
{
    pendingAction = PendingAction::Push;
    pendingScreen = std::move(screen);
}

void ScreenController::popScreen()
{
    pendingAction = PendingAction::Pop;
    pendingScreen = nullptr;
}

void ScreenController::replaceScreen(std::unique_ptr<ScreenI>&& screen)
{
    pendingAction = PendingAction::Replace;
    pendingScreen = std::move(screen);
}

void ScreenController::applyPendingTransition()
{
    switch(pendingAction)
    {
        case PendingAction::Push:
            screenStack.push_back(std::move(pendingScreen));
            break;
        case PendingAction::Pop:
            if(screenStack.size() > 1)
            {
                screenStack.pop_back();
            }
            break;
        case PendingAction::Replace:
            screenStack.clear();
            screenStack.push_back(std::move(pendingScreen));
            break;
        case PendingAction::None:
            break;
    }
    pendingAction = PendingAction::None;
    pendingScreen = nullptr;
}

void ScreenController::update(float dt)
{
    applyPendingTransition();
    if(not screenStack.empty())
    {
        screenStack.back()->update(dt);
    }
}

void ScreenController::display()
{
    screenRenderer.clear();
    for(auto& screen: screenStack)
    {
        screen->display();
    }
    screenRenderer.display();
}
