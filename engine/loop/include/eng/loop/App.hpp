#pragma once

#include <cstdint>
#include <eng/input/ActionMap.hpp>
#include <eng/input/InputState.hpp>
#include <eng/loop/ClockI.hpp>
#include <eng/loop/FixedStepLoop.hpp>
#include <eng/loop/PlatformI.hpp>

namespace eng
{

class RenderQueue;
class SceneStack;

class App
{
public:
    App(PlatformI& platform, ClockI& clock, ActionMap map, SceneStack& stack, RenderQueue& queue);

    void run();
    void runFrames(std::uint32_t n);
    float lastAlpha() const;

private:
    void runFrame();

    PlatformI& platform;
    ClockI& clock;
    ActionMap map;
    SceneStack& stack;
    RenderQueue& queue;
    InputState input;
    FixedStepLoop loop;
    float alpha{};
};

} // namespace eng
