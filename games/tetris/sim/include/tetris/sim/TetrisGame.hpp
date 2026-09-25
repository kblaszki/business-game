#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <sgl/core/Time.hpp>
#include <tetris/sim/Bag.hpp>
#include <tetris/sim/Events.hpp>
#include <tetris/sim/Grid.hpp>
#include <tetris/sim/Piece.hpp>
#include <tetris/sim/Scoring.hpp>
#include <tetris/sim/Tuning.hpp>
#include <tetris/sim/Types.hpp>
#include <vector>

namespace sgl::tetris
{

struct TetrisInput
{
    bool leftPressed{};
    bool leftHeld{};
    bool rightPressed{};
    bool rightHeld{};
    bool softDropHeld{};
    bool hardDrop{};
    bool rotateCw{};
    bool rotateCcw{};
    bool hold{};
};

class TetrisGame
{
public:
    explicit TetrisGame(std::uint64_t seed, Tuning tuning = {});

    std::vector<TetrisEvent> step(const TetrisInput& input, sgl::Seconds dt);

    [[nodiscard]] const Grid& grid() const noexcept;
    [[nodiscard]] const std::optional<Piece>& active() const noexcept;
    [[nodiscard]] std::optional<Piece> ghost() const;
    [[nodiscard]] std::optional<PieceType> held() const noexcept;
    [[nodiscard]] std::array<PieceType, 5> nextQueue() const;
    [[nodiscard]] const Score& score() const noexcept;
    [[nodiscard]] bool over() const noexcept;

private:
    enum class Horizontal : std::int8_t
    {
        None = 0,
        Left = -1,
        Right = 1
    };

    [[nodiscard]] bool grounded() const;
    [[nodiscard]] int lowestRow(const Piece& piece) const;
    void noteLowest(const Piece& piece);
    bool tryApplyShift(Coord delta, std::vector<TetrisEvent>& events);
    bool tryApplyRotate(Turn turn, std::vector<TetrisEvent>& events);
    void handleHold(const TetrisInput& input, std::vector<TetrisEvent>& events);
    void handleRotations(const TetrisInput& input, std::vector<TetrisEvent>& events);
    void handleHorizontal(const TetrisInput& input, sgl::Seconds dt, std::vector<TetrisEvent>& events);
    void handleDrops(const TetrisInput& input, sgl::Seconds dt, std::vector<TetrisEvent>& events);
    void handleGravity(sgl::Seconds dt, bool soft, std::vector<TetrisEvent>& events);
    void handleLock(sgl::Seconds dt, std::vector<TetrisEvent>& events);
    void lockActive(std::vector<TetrisEvent>& events, bool hardDropped);
    void clearAndSpawn(std::vector<TetrisEvent>& events);
    bool spawnNext(std::vector<TetrisEvent>& events);
    bool spawnType(PieceType type, std::vector<TetrisEvent>& events);
    void resetDas();
    [[nodiscard]] Horizontal resolveDirection(const TetrisInput& input);

    Tuning tuning_{};
    Bag bag_;
    Grid grid_{};
    std::optional<Piece> active_{};
    std::optional<PieceType> held_{};
    Score score_{};
    bool over_{false};
    bool holdAvailable_{true};

    sgl::Seconds fallTimer_{};
    sgl::Seconds lockTimer_{};
    int lockResets_{0};
    int lowestRow_{-1};

    Horizontal dasDir_{Horizontal::None};
    sgl::Seconds dasTimer_{};
    sgl::Seconds arrTimer_{};
    bool dasCharged_{false};
    Horizontal lastPressed_{Horizontal::None};
    bool prevLeftHeld_{false};
    bool prevRightHeld_{false};
};

} // namespace sgl::tetris
