#pragma once

#include <cstdint>
#include <sgl/input/ActionMap.hpp>
#include <sgl/input/InputState.hpp>
#include <sgl/loop/ClockI.hpp>
#include <sgl/loop/FixedStepLoop.hpp>
#include <sgl/loop/PlatformI.hpp>

namespace sgl
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

} // namespace sgl
