#pragma once

#include <cstddef>
#include <eng/core/Time.hpp>
#include <eng/input/InputState.hpp>
#include <eng/scene/SceneI.hpp>
#include <eng/scene/SceneRequest.hpp>
#include <memory>
#include <vector>

namespace eng
{

class RenderQueue;
class SceneContext;

class SceneStack
{
public:
    explicit SceneStack(SceneFactory pauseOverlay);

    SceneStack(const SceneStack&) = delete;
    SceneStack& operator=(const SceneStack&) = delete;
    SceneStack(SceneStack&&) = default;
    SceneStack& operator=(SceneStack&&) = default;

    void push(std::unique_ptr<SceneI> scene);
    void update(const InputState& input, Seconds dt);
    void render(RenderQueue& queue) const;

    [[nodiscard]] bool quitRequested() const;
    [[nodiscard]] bool empty() const;
    [[nodiscard]] std::size_t size() const;

private:
    friend class SceneContext;

    void enqueue(SceneRequest request);
    void tryEnqueuePause();
    void applyRequests();

    SceneFactory pauseOverlay_;
    std::vector<std::unique_ptr<SceneI>> scenes_;
    std::vector<SceneRequest> pending_;
    bool updating_{false};
    bool pauseQueued_{false};
    bool quitWanted_{false};
};

} // namespace eng
