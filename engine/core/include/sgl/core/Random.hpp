#pragma once

#include <cstdint>

namespace sgl
{

// PCG-XSH-RR 64/32 (pcg32). Seeded streams for deterministic sim / fx randomness.
class Pcg32
{
public:
    explicit Pcg32(std::uint64_t seed, std::uint64_t stream = 1) noexcept
        : state_{0}
        , inc_{(stream << 1u) | 1u}
    {
        (void)nextU32();
        state_ += seed;
        (void)nextU32();
    }

    [[nodiscard]] std::uint32_t nextU32() noexcept
    {
        const std::uint64_t oldstate = state_;
        state_ = oldstate * 6364136223846793005ULL + inc_;
        const auto xorshifted = static_cast<std::uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
        const auto rot = static_cast<std::uint32_t>(oldstate >> 59u);
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31u));
    }

    // Unbiased via rejection sampling. bound == 0 returns 0.
    [[nodiscard]] std::uint32_t uniformBelow(std::uint32_t bound) noexcept
    {
        if(bound == 0u)
        {
            return 0u;
        }
        const std::uint32_t threshold = (~bound + 1u) % bound; // -bound % bound
        for(;;)
        {
            const std::uint32_t r = nextU32();
            if(r >= threshold)
            {
                return r % bound;
            }
        }
    }

    [[nodiscard]] float uniformFloat(float lo, float hi) noexcept
    {
        constexpr float kScale = 1.f / 4294967296.f;
        return lo + (hi - lo) * (static_cast<float>(nextU32()) * kScale);
    }

private:
    std::uint64_t state_{};
    std::uint64_t inc_{};
};

} // namespace sgl
