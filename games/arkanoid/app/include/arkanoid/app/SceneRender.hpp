#pragma once

#include <arkanoid/sim/State.hpp>
#include <array>
#include <sgl/render/AssetIds.hpp>
#include <sgl/render/RenderQueue.hpp>

namespace sgl::arkanoid
{

struct TextureIds
{
    sgl::TextureId background;
    sgl::TextureId brick;
    sgl::TextureId paddle;
    sgl::TextureId ball;
    std::array<sgl::TextureId, 4> capsule;
};

void renderState(const State& state, const TextureIds& textures, sgl::RenderQueue& queue);

} // namespace sgl::arkanoid
