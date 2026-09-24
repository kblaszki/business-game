#include <eng/scene/SceneContext.hpp>
#include <eng/scene/SceneStack.hpp>
#include <utility>
#include <variant>

namespace eng
{

namespace
{

struct RequestApplier
{
    std::vector<std::unique_ptr<SceneI>>& scenes;
    SceneFactory& pauseOverlay;
    bool& quitWanted;

    void operator()(PushScene& push)
    {
        if(auto scene = push.make())
        {
            scenes.push_back(std::move(scene));
        }
    }

    void operator()(PopScene)
    {
        if(!scenes.empty())
        {
            scenes.pop_back();
        }
    }

    void operator()(ReplaceScene& replace)
    {
        if(!scenes.empty())
        {
            scenes.pop_back();
        }
        if(auto scene = replace.make())
        {
            scenes.push_back(std::move(scene));
        }
    }

    void operator()(RequestPause)
    {
        if(auto overlay = pauseOverlay())
        {
            scenes.push_back(std::move(overlay));
        }
    }

    void operator()(QuitApp)
    {
        quitWanted = true;
    }
};
} // namespace

SceneStack::SceneStack(SceneFactory pauseOverlay)
    : pauseOverlay_{std::move(pauseOverlay)}
{
}

void SceneStack::push(std::unique_ptr<SceneI> scene)
{
    if(!scene)
    {
        return;
    }
    scenes_.push_back(std::move(scene));
}

void SceneStack::update(const InputState& input, Seconds dt)
{
    updating_ = true;
    pauseQueued_ = false;

    if(input.focusLost())
    {
        tryEnqueuePause();
    }

    SceneContext ctx{input, *this};
    for(std::size_t i = scenes_.size(); i > 0; --i)
    {
        SceneI& scene = *scenes_[i - 1];
        scene.update(ctx, dt);
        if(scene.traits().blocksUpdate)
        {
            break;
        }
    }

    updating_ = false;
    applyRequests();
}

void SceneStack::render(RenderQueue& queue) const
{
    if(scenes_.empty())
    {
        return;
    }

    std::size_t start{0};
    for(std::size_t i = scenes_.size(); i > 0; --i)
    {
        if(scenes_[i - 1]->traits().opaque)
        {
            start = i - 1;
            break;
        }
    }

    for(std::size_t i = start; i < scenes_.size(); ++i)
    {
        scenes_[i]->render(queue);
    }
}

bool SceneStack::quitRequested() const
{
    return quitWanted_;
}

bool SceneStack::empty() const
{
    return scenes_.empty();
}

std::size_t SceneStack::size() const
{
    return scenes_.size();
}

void SceneStack::enqueue(SceneRequest request)
{
    if(std::holds_alternative<RequestPause>(request))
    {
        tryEnqueuePause();
    }
    else
    {
        pending_.push_back(std::move(request));
    }

    if(!updating_)
    {
        applyRequests();
    }
}

void SceneStack::tryEnqueuePause()
{
    if(pauseQueued_ || scenes_.empty())
    {
        return;
    }
    if(!scenes_.back()->traits().pausable)
    {
        return;
    }

    pauseQueued_ = true;
    pending_.emplace_back(RequestPause{});
}

void SceneStack::applyRequests()
{
    for(SceneRequest& request: pending_)
    {
        std::visit(RequestApplier{scenes_, pauseOverlay_, quitWanted_}, request);
    }
    pending_.clear();
    pauseQueued_ = false;
}

} // namespace eng
