#include <algorithm>
#include <cmath>
#include <cstdint>
#include <sgl/fx/ParticleSystem.hpp>

namespace sgl
{
namespace
{

constexpr float kDegToRad = 3.14159265358979323846f / 180.f;

} // namespace

ParticleSystem::ParticleSystem(std::size_t capacity)
    : capacity_{capacity}
{
    particles_.reserve(capacity);
    gravities_.reserve(capacity);
}

void ParticleSystem::emit(const BurstParams& params, Pcg32& rng)
{
    for(std::size_t i = 0; i < params.count; ++i)
    {
        if(particles_.size() >= capacity_)
        {
            break;
        }

        const float speed = rng.uniformFloat(params.speedMin, params.speedMax);
        const float angleDeg = rng.uniformFloat(params.angleMinDeg, params.angleMaxDeg);
        const float angleRad = angleDeg * kDegToRad;
        const float life = rng.uniformFloat(params.lifeMin, params.lifeMax);

        particles_.push_back(Particle{
            .pos = params.origin,
            .vel = {speed * std::cos(angleRad), speed * std::sin(angleRad)},
            .size = params.size,
            .rotationDeg = 0.f,
            .spinDeg = 0.f,
            .life = life,
            .maxLife = life,
            .color = params.color,
        });
        gravities_.push_back(params.gravity);
    }
}

void ParticleSystem::update(Seconds dt)
{
    const float dtSec = dt.count();
    for(std::size_t i = 0; i < particles_.size();)
    {
        Particle& p = particles_[i];
        p.vel += gravities_[i] * dtSec;
        p.pos += p.vel * dtSec;
        p.rotationDeg += p.spinDeg * dtSec;
        p.life -= dtSec;
        if(p.life <= 0.f)
        {
            particles_[i] = particles_.back();
            gravities_[i] = gravities_.back();
            particles_.pop_back();
            gravities_.pop_back();
            continue;
        }
        ++i;
    }
}

void ParticleSystem::render(RenderQueue& queue, Layer layer) const
{
    for(const Particle& p: particles_)
    {
        const float half = p.size * 0.5f;
        const float t = (p.maxLife > 0.f) ? (p.life / p.maxLife) : 0.f;
        const auto alpha = static_cast<std::uint8_t>(
            std::clamp(static_cast<int>(std::lround(static_cast<float>(p.color.a) * t)), 0, 255));
        Color fill = p.color;
        fill.a = alpha;

        queue.push(layer,
                   0.f,
                   RectCmd{
                       .rect = {.pos = {p.pos.x - half, p.pos.y - half}, .size = {p.size, p.size}},
                       .fill = fill,
                       .rotationDeg = p.rotationDeg,
                   });
    }
}

} // namespace sgl
