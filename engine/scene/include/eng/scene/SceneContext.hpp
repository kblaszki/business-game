#pragma once

#include <eng/input/InputState.hpp>
#include <eng/scene/SceneRequest.hpp>

namespace eng
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

} // namespace eng
