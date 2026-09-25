#pragma once

#include <cstddef>
#include <sgl/core/Color.hpp>
#include <sgl/core/Random.hpp>
#include <sgl/core/Time.hpp>
#include <sgl/core/Vec2.hpp>
#include <sgl/render/DrawCommand.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <vector>

namespace sgl
{

struct Particle
{
    Vec2f pos{};
    Vec2f vel{};
    float size{};
    float rotationDeg{};
    float spinDeg{};
    float life{};
    float maxLife{};
    Color color{};
};

struct BurstParams
{
    Vec2f origin{};
    std::size_t count{};
    float speedMin{};
    float speedMax{};
    float angleMinDeg{};
    float angleMaxDeg{};
    float lifeMin{};
    float lifeMax{};
    float size{};
    Color color{};
    Vec2f gravity{};
};

class ParticleSystem
{
public:
    explicit ParticleSystem(std::size_t capacity);

    void emit(const BurstParams& params, Pcg32& rng);
    void update(Seconds dt);
    void render(RenderQueue& queue, Layer layer) const;

    [[nodiscard]] std::size_t size() const noexcept
    {
        return particles_.size();
    }

private:
    std::size_t capacity_{};
    std::vector<Particle> particles_;
    std::vector<Vec2f> gravities_;
};

} // namespace sgl
