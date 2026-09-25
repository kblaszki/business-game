#pragma once

#include <arkanoid/app/Bindings.hpp>
#include <arkanoid/app/SceneRender.hpp>
#include <arkanoid/sim/State.hpp>
#include <sgl/render/AssetIds.hpp>
#include <sgl/scene/SceneRequest.hpp>

namespace sgl::arkanoid
{

struct AppServices
{
    const Actions& actions;
    const TextureIds& textures;
    sgl::FontId font;
};

[[nodiscard]] sgl::SceneFactory mainMenu(const AppServices& services);
[[nodiscard]] sgl::SceneFactory gameplay(const AppServices& services, StageId stage);
[[nodiscard]] sgl::SceneFactory pause(const AppServices& services);

} // namespace sgl::arkanoid
