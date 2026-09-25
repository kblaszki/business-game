#include <ranges>
#include <sgl/render/DrawCommand.hpp>
#include <tetris/app/BoardRender.hpp>
#include <tetris/app/Theme.hpp>
#include <tetris/sim/Grid.hpp>
#include <tetris/sim/Piece.hpp>
#include <tetris/sim/Shapes.hpp>

namespace sgl::tetris
{
namespace
{

[[nodiscard]] sgl::Rect<float> cellRect(sgl::Vec2f origin, float cell, Coord c, int visibleRows)
{
    return sgl::Rect<float>{
        .pos = {origin.x + static_cast<float>(c.x) * cell, origin.y + static_cast<float>(visibleRows - 1 - c.y) * cell},
        .size = {cell, cell},
    };
}

[[nodiscard]] sgl::Rect<float> previewCellRect(sgl::Vec2f origin, float cell, Coord local)
{
    return sgl::Rect<float>{
        .pos = {origin.x + static_cast<float>(local.x) * cell, origin.y + static_cast<float>(3 - local.y) * cell},
        .size = {cell, cell},
    };
}

void drawPieceType(
    PieceType type, sgl::Vec2f origin, float cell, sgl::Layer layer, float depthBase, sgl::RenderQueue& queue)
{
    const Cells local = shape(type, Rotation::Spawn);
    const sgl::Color fill = pieceColor(type);
    for(const Coord c: local)
    {
        queue.push(layer,
                   depthBase + static_cast<float>(c.y),
                   sgl::RectCmd{.rect = previewCellRect(origin, cell, c), .fill = fill});
    }
}

void drawCells(const Cells& cells,
               PieceType type,
               const Layout& layout,
               sgl::Layer layer,
               float depthBias,
               bool ghost,
               sgl::RenderQueue& queue)
{
    const sgl::Color fill = ghost ? ghostColor(type) : pieceColor(type);
    for(const Coord c: cells)
    {
        if(c.y < 0 || c.y >= Grid::kVisibleHeight)
        {
            continue;
        }
        queue.push(
            layer,
            depthBias + static_cast<float>(c.y),
            sgl::RectCmd{.rect = cellRect(layout.wellOrigin, layout.cell, c, Grid::kVisibleHeight), .fill = fill});
    }
}

} // namespace

Layout defaultLayout()
{
    const float wellW = static_cast<float>(Grid::kWidth) * cellSize;
    const float wellH = static_cast<float>(Grid::kVisibleHeight) * cellSize;
    const sgl::Vec2f wellOrigin{(designWidth - wellW) * 0.5f, (designHeight - wellH) * 0.5f};
    return Layout{
        .cell = cellSize,
        .wellOrigin = wellOrigin,
        .holdOrigin = {wellOrigin.x - 140.f, wellOrigin.y + 40.f},
        .nextOrigin = {wellOrigin.x + wellW + 40.f, wellOrigin.y + 40.f},
        .previewCell = 20.f,
    };
}

void renderBoard(const TetrisGame& game, const Layout& layout, sgl::RenderQueue& queue)
{
    const float wellW = static_cast<float>(Grid::kWidth) * layout.cell;
    const float wellH = static_cast<float>(Grid::kVisibleHeight) * layout.cell;

    queue.push(sgl::Layer::Background,
               0.f,
               sgl::RectCmd{
                   .rect = {.pos = {0.f, 0.f}, .size = {designWidth, designHeight}},
                   .fill = wellFill,
               });
    queue.push(sgl::Layer::World,
               -1.f,
               sgl::RectCmd{
                   .rect = {.pos = layout.wellOrigin, .size = {wellW, wellH}},
                   .fill = sgl::Color{8, 10, 18},
                   .outline = wellBorder,
                   .outlineThickness = 2.f,
               });

    const auto visibleRows = game.grid().rows() | std::views::take(Grid::kVisibleHeight);
    int rowIndex = 0;
    for(const Grid::Row& row: visibleRows)
    {
        for(int x = 0; x < Grid::kWidth; ++x)
        {
            if(!row[static_cast<std::size_t>(x)].has_value())
            {
                continue;
            }
            const PieceType type = *row[static_cast<std::size_t>(x)];
            const Coord c{x, rowIndex};
            queue.push(sgl::Layer::World,
                       static_cast<float>(rowIndex),
                       sgl::RectCmd{.rect = cellRect(layout.wellOrigin, layout.cell, c, Grid::kVisibleHeight),
                                    .fill = pieceColor(type)});
        }
        ++rowIndex;
    }

    if(const std::optional<Piece> ghost = game.ghost())
    {
        drawCells(cells(*ghost), ghost->type, layout, sgl::Layer::Actors, 0.f, true, queue);
    }
    if(game.active().has_value())
    {
        drawCells(cells(*game.active()), game.active()->type, layout, sgl::Layer::Actors, 100.f, false, queue);
    }

    queue.push(sgl::Layer::Hud,
               0.f,
               sgl::RectCmd{
                   .rect = {.pos = {layout.holdOrigin.x - 8.f, layout.holdOrigin.y - 8.f}, .size = {120.f, 120.f}},
                   .fill = panelFill,
               });
    if(game.held().has_value())
    {
        drawPieceType(*game.held(), layout.holdOrigin, layout.previewCell, sgl::Layer::Hud, 1.f, queue);
    }

    queue.push(sgl::Layer::Hud,
               0.f,
               sgl::RectCmd{
                   .rect = {.pos = {layout.nextOrigin.x - 8.f, layout.nextOrigin.y - 8.f}, .size = {120.f, 520.f}},
                   .fill = panelFill,
               });
    const auto next = game.nextQueue();
    for(std::size_t i = 0; i < next.size(); ++i)
    {
        const sgl::Vec2f origin{layout.nextOrigin.x, layout.nextOrigin.y + static_cast<float>(i) * 96.f};
        drawPieceType(next[i], origin, layout.previewCell, sgl::Layer::Hud, 2.f + static_cast<float>(i), queue);
    }
}

} // namespace sgl::tetris
