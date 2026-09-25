#include <arkanoid/sim/Levels.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <array>
#include <cstddef>

namespace sgl::arkanoid
{
namespace
{

constexpr std::size_t kCells = static_cast<std::size_t>(levelColumns) * static_cast<std::size_t>(levelRows);

constexpr sgl::Color stageColors[levelRows]{
    {220, 70, 80},
    {230, 140, 50},
    {230, 200, 60},
    {80, 180, 90},
    {60, 140, 220},
    {150, 80, 200},
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
    for(std::uint32_t row = 0; row < levelRows; ++row)
    {
        for(std::uint32_t col = 0; col < levelColumns; ++col)
        {
            cells[row * levelColumns + col] = ((row + col) % 2 == 0) ? std::uint8_t{1} : std::uint8_t{0};
        }
    }
    return cells;
}

consteval std::array<std::uint8_t, kCells> makeStage3Mask()
{
    std::array<std::uint8_t, kCells> cells{};
    for(std::uint32_t row = 0; row < levelRows; ++row)
    {
        for(std::uint32_t col = 0; col < levelColumns; ++col)
        {
            const bool border = row == 0 || row + 1 == levelRows || col == 0 || col + 1 == levelColumns;
            cells[row * levelColumns + col] = border ? std::uint8_t{1} : std::uint8_t{0};
        }
    }
    return cells;
}

constexpr auto stage1Mask = makeStage1Mask();
constexpr auto stage2Mask = makeStage2Mask();
constexpr auto stage3Mask = makeStage3Mask();

const LevelData stage1{StageId::Stage1, levelColumns, levelRows, stageColors, stage1Mask};
const LevelData stage2{StageId::Stage2, levelColumns, levelRows, stageColors, stage2Mask};
const LevelData stage3{StageId::Stage3, levelColumns, levelRows, stageColors, stage3Mask};

[[nodiscard]] bool occupiesCell(const LevelData& data, std::uint32_t row, std::uint32_t col)
{
    if(data.mask.empty())
    {
        return true;
    }

    const std::size_t index = static_cast<std::size_t>(row) * data.columns + col;
    return index < data.mask.size() && data.mask[index] == 1;
}

void stickBallToPaddle(Ball& ball, const Paddle& paddle)
{
    ball.pos = {paddle.x + (paddle.width - ballDiameter) * 0.5f, paddleY - ballDiameter};
    ball.vel = {};
    ball.mode = BallMode::Stuck;
}

} // namespace

const LevelData& level(StageId id)
{
    switch(id)
    {
        case StageId::Stage2:
            return stage2;
        case StageId::Stage3:
            return stage3;
        case StageId::Stage1:
        default:
            return stage1;
    }
}

State makeState(StageId id, std::uint32_t score, std::uint32_t lives)
{
    const LevelData& data = level(id);
    State state{};
    state.stage = data.id;
    state.score = score;
    state.lives = lives;
    state.paddle.x = (designWidth - paddleWidth) * 0.5f;
    state.paddle.width = paddleWidth;

    const float gridW = static_cast<float>(data.columns) * (brickWidth + brickGap) - brickGap;
    const float originX = (designWidth - gridW) * 0.5f;

    std::size_t liveIndex = 0;
    for(std::uint32_t row = 0; row < data.rows; ++row)
    {
        const sgl::Color tint =
            data.rowColors.empty() ? sgl::Color{255, 255, 255} : data.rowColors[row % data.rowColors.size()];
        for(std::uint32_t col = 0; col < data.columns; ++col)
        {
            if(!occupiesCell(data, row, col))
            {
                continue;
            }

            Brick brick{};
            brick.box.pos = {originX + static_cast<float>(col) * (brickWidth + brickGap),
                             brickOriginY + static_cast<float>(row) * (brickHeight + brickGap)};
            brick.box.size = {brickWidth, brickHeight};
            brick.tint = tint;
            brick.alive = true;
            if(liveIndex % 4 == 0)
            {
                brick.drop = static_cast<PowerUpKind>((liveIndex / 4) % 4);
            }
            state.bricks.push_back(brick);
            ++liveIndex;
        }
    }

    Ball ball{};
    stickBallToPaddle(ball, state.paddle);
    state.balls.push_back(ball);
    return state;
}

} // namespace sgl::arkanoid
