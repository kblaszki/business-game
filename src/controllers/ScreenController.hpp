/* Created by kblaszki */
#pragma once

#include "ScreenControllerI.hpp"

#include <screens/ScreenI.hpp>
#include <window/ScreenRendererI.hpp>

#include <functional>
#include <memory>
#include <vector>

class ScreenController : public ScreenControllerI
{
public:
    using ScreenFactory = std::function<std::unique_ptr<ScreenI>(ScreenUpdaterI&)>;

    ScreenController(ScreenRendererI& screenRenderer, ScreenFactory makeInitialScreen);

    void pushScreen(std::unique_ptr<ScreenI>&& screen) override;
    void popScreen() override;
    void replaceScreen(std::unique_ptr<ScreenI>&& screen) override;
    void update(float dt) override;
    void display() override;

private:
    enum class PendingAction
    {
        None,
        Push,
        Pop,
        Replace
    };

    void applyPendingTransition();

    std::vector<std::unique_ptr<ScreenI>> screenStack;
    PendingAction pendingAction{PendingAction::None};
    std::unique_ptr<ScreenI> pendingScreen;
    ScreenRendererI& screenRenderer;
};
