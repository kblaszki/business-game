#pragma once

#include <cstdint>
#include <sgl/render/AssetIds.hpp>
#include <sgl/scene/SceneRequest.hpp>
#include <tetris/app/Bindings.hpp>

namespace sgl::tetris
{

struct AppServices
{
    const Actions& actions;
    sgl::FontId font;
    std::uint64_t seed{};
};

[[nodiscard]] sgl::SceneFactory mainMenu(const AppServices& services);
[[nodiscard]] sgl::SceneFactory play(const AppServices& services);
[[nodiscard]] sgl::SceneFactory pause(const AppServices& services);
[[nodiscard]] sgl::SceneFactory gameOver(const AppServices& services, std::uint32_t score);

} // namespace sgl::tetris
