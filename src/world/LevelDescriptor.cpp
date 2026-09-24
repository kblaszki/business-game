#include "LevelDescriptor.hpp"

#include <Game.hpp>

#include <array>

namespace
{
constexpr std::uint32_t kColumns = 10;
constexpr std::uint32_t kRows = 6;
constexpr std::size_t kCells = kColumns * kRows;

const BrickRow stageColors[]{
    {sf::Color{220, 70, 80}},
    {sf::Color{230, 140, 50}},
    {sf::Color{230, 200, 60}},
    {sf::Color{80, 180, 90}},
    {sf::Color{60, 140, 220}},
    {sf::Color{150, 80, 200}},
};

consteval std::array<std::uint8_t, kCells> makeStage1Mask()
{
    std::array<std::uint8_t, kCells> cells{};
    cells.fill(1);
    return cells;
}

consteval std::array<std::uint8_t, kCells> makeStage2Mask()
{
    std::array<std::uint8_t, kCells> cells{};
    for(std::uint32_t row = 0; row < kRows; ++row)
    {
        for(std::uint32_t col = 0; col < kColumns; ++col)
        {
            cells[row * kColumns + col] = ((row + col) % 2 == 0) ? std::uint8_t{1} : std::uint8_t{0};
        }
    }
    return cells;
}

consteval std::array<std::uint8_t, kCells> makeStage3Mask()
{
    std::array<std::uint8_t, kCells> cells{};
    for(std::uint32_t row = 0; row < kRows; ++row)
    {
        for(std::uint32_t col = 0; col < kColumns; ++col)
        {
            const bool border = row == 0 || row + 1 == kRows || col == 0 || col + 1 == kColumns;
            cells[row * kColumns + col] = border ? std::uint8_t{1} : std::uint8_t{0};
        }
    }
    return cells;
}

constexpr auto stage1Mask = makeStage1Mask();
constexpr auto stage2Mask = makeStage2Mask();
constexpr auto stage3Mask = makeStage3Mask();

const LevelDescriptor stage1{LevelId::Stage1, kColumns, kRows, stageColors, stage1Mask};
const LevelDescriptor stage2{LevelId::Stage2, kColumns, kRows, stageColors, stage2Mask};
const LevelDescriptor stage3{LevelId::Stage3, kColumns, kRows, stageColors, stage3Mask};

[[nodiscard]] bool occupiesCell(const LevelDescriptor& descriptor, std::uint32_t row, std::uint32_t col)
{
    if(descriptor.mask.empty())
    {
        return true;
    }

    const std::size_t index = static_cast<std::size_t>(row) * descriptor.columns + col;
    return index < descriptor.mask.size() && descriptor.mask[index] == 1;
}
} // namespace

const LevelDescriptor& levelDescriptor(LevelId id)
{
    switch(id)
    {
        case LevelId::Stage2:
            return stage2;
        case LevelId::Stage3:
            return stage3;
        case LevelId::Stage1:
        default:
            return stage1;
    }
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
            if(!occupiesCell(descriptor, row, col))
            {
                continue;
            }

            world.addBrick({originX + static_cast<float>(col) * (brickW + gap),
                            originY + static_cast<float>(row) * (brickH + gap)},
                           tint);
        }
    }

    return world;
}
