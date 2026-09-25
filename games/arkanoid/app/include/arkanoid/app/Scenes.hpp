#pragma once

#include <arkanoid/app/Bindings.hpp>
#include <arkanoid/app/SceneRender.hpp>
#include <arkanoid/sim/State.hpp>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <sgl/render/AssetIds.hpp>
#include <sgl/save/HighScoreTable.hpp>
#include <sgl/scene/SceneRequest.hpp>

namespace sgl::arkanoid
{

inline constexpr std::size_t kHighScoreCapacity{5};

enum class Outcome
{
    Won,
    Lost
};

struct AppServices
{
    const Actions& actions;
    const TextureIds& textures;
    sgl::FontId font;
    sgl::HighScoreTable& highScores;
    std::filesystem::path scoresPath;
};

[[nodiscard]] sgl::SceneFactory mainMenu(const AppServices& services);
[[nodiscard]] sgl::SceneFactory gameplay(const AppServices& services, StageId stage);
[[nodiscard]] sgl::SceneFactory pause(const AppServices& services);
[[nodiscard]] sgl::SceneFactory result(const AppServices& services,
                                       Outcome outcome,
                                       std::uint32_t score,
                                       std::optional<std::size_t> rank,
                                       bool saveFailed);

} // namespace sgl::arkanoid
