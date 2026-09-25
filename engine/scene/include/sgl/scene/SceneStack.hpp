#pragma once

#include <cstddef>
#include <memory>
#include <sgl/core/Time.hpp>
#include <sgl/input/InputState.hpp>
#include <sgl/scene/SceneI.hpp>
#include <sgl/scene/SceneRequest.hpp>
#include <vector>

namespace sgl
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
    [[nodiscard]] SceneI* top() const;

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

} // namespace sgl
