#pragma once

#include <sgl/core/Vec2.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <tetris/app/Theme.hpp>
#include <tetris/sim/TetrisGame.hpp>

namespace sgl::tetris
{

struct Layout
{
    float cell{cellSize};
    sgl::Vec2f wellOrigin{};
    sgl::Vec2f holdOrigin{};
    sgl::Vec2f nextOrigin{};
    float previewCell{20.f};
};

[[nodiscard]] Layout defaultLayout();

void renderBoard(const TetrisGame& game, const Layout& layout, sgl::RenderQueue& queue);

} // namespace sgl::tetris
