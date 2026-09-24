#pragma once

#include <eng/scene/SceneI.hpp>
#include <functional>
#include <memory>
#include <variant>

namespace eng
{

using SceneFactory = std::move_only_function<std::unique_ptr<SceneI>()>;

struct PushScene
{
    SceneFactory make;
};

struct PopScene
{
};

struct ReplaceScene
{
    SceneFactory make;
};

struct RequestPause
{
};

struct QuitApp
{
};

using SceneRequest = std::variant<PushScene, PopScene, ReplaceScene, RequestPause, QuitApp>;

} // namespace eng
