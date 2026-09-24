#pragma once

#include <arkanoid/sim/State.hpp>
#include <eng/render/AssetIds.hpp>
#include <eng/render/RenderQueue.hpp>

#include <array>

namespace arkanoid
{

struct TextureIds
{
    eng::TextureId background;
    eng::TextureId brick;
    eng::TextureId paddle;
    eng::TextureId ball;
    std::array<eng::TextureId, 4> capsule;
};

void renderState(const State& state, const TextureIds& textures, eng::RenderQueue& queue);

} // namespace arkanoid
