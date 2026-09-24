#pragma once

#include <arkanoid/app/Bindings.hpp>
#include <arkanoid/app/SceneRender.hpp>
#include <arkanoid/sim/State.hpp>
#include <eng/render/AssetIds.hpp>
#include <eng/scene/SceneRequest.hpp>

namespace arkanoid
{

struct AppServices
{
    const Actions& actions;
    const TextureIds& textures;
    eng::FontId font;
};

[[nodiscard]] eng::SceneFactory mainMenu(const AppServices& services);
[[nodiscard]] eng::SceneFactory gameplay(const AppServices& services, StageId stage);
[[nodiscard]] eng::SceneFactory pause(const AppServices& services);

} // namespace arkanoid
