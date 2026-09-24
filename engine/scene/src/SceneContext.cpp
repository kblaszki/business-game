#include <eng/scene/SceneContext.hpp>
#include <eng/scene/SceneStack.hpp>

namespace eng
{

SceneContext::SceneContext(const InputState& input, SceneStack& stack)
    : input_{input}
    , stack_{stack}
{
}

const InputState& SceneContext::input() const
{
    return input_;
}

void SceneContext::request(SceneRequest request)
{
    stack_.enqueue(std::move(request));
}

} // namespace eng
