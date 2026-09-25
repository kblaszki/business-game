#pragma once

#include <sgl/input/InputState.hpp>
#include <sgl/scene/SceneRequest.hpp>

namespace sgl
{

class SceneStack;

class SceneContext
{
public:
    [[nodiscard]] const InputState& input() const;
    void request(SceneRequest request);

private:
    friend class SceneStack;

    SceneContext(const InputState& input, SceneStack& stack);

    const InputState& input_;
    SceneStack& stack_;
};

} // namespace sgl
