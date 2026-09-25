#pragma once

#include <array>
#include <cstdint>
#include <sgl/core/Random.hpp>
#include <tetris/sim/Types.hpp>
#include <vector>

namespace sgl::tetris
{

class Bag
{
public:
    explicit Bag(std::uint64_t seed);

    [[nodiscard]] PieceType next();
    [[nodiscard]] std::array<PieceType, 5> preview() const;

private:
    void refill();
    void ensure(std::size_t count);

    sgl::Pcg32 rng_;
    std::vector<PieceType> queue_{};
};

} // namespace sgl::tetris
