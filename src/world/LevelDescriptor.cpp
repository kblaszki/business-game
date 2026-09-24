#include "LevelDescriptor.hpp"

#include <Game.hpp>

namespace
{
const BrickRow stage1Colors[]{
    {sf::Color{220, 70, 80}},
    {sf::Color{230, 140, 50}},
    {sf::Color{230, 200, 60}},
    {sf::Color{80, 180, 90}},
    {sf::Color{60, 140, 220}},
    {sf::Color{150, 80, 200}},
};

const LevelDescriptor stage1{LevelId::Stage1, 10, 6, stage1Colors};
} // namespace

const LevelDescriptor& levelDescriptor(LevelId id)
{
    switch(id)
    {
        case LevelId::Stage1:
            return stage1;
    }

    return stage1;
}

World makeWorld(const LevelDescriptor& descriptor)
{
    World world;
    constexpr float brickW = 110.f;
    constexpr float brickH = 28.f;
    constexpr float gap = 8.f;
    const float gridW = static_cast<float>(descriptor.columns) * (brickW + gap) - gap;
    const float originX = (static_cast<float>(Game::DESIGN_SIZE.x) - gridW) * 0.5f;
    constexpr float originY = 72.f;

    for(std::uint32_t row = 0; row < descriptor.rows; ++row)
    {
        const sf::Color tint =
            descriptor.palette.empty() ? sf::Color::White : descriptor.palette[row % descriptor.palette.size()].color;
        for(std::uint32_t col = 0; col < descriptor.columns; ++col)
        {
            world.addBrick({originX + static_cast<float>(col) * (brickW + gap),
                            originY + static_cast<float>(row) * (brickH + gap)},
                           tint);
        }
    }

    return world;
}
