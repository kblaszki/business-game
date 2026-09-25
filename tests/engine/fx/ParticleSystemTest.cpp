#include <gtest/gtest.h>
#include <sgl/core/Random.hpp>
#include <sgl/fx/ParticleSystem.hpp>
#include <sgl/render/DrawCommand.hpp>
#include <sgl/render/RenderQueue.hpp>
#include <variant>

namespace
{

sgl::BurstParams makeBurst(std::size_t count, sgl::Color color = {255, 128, 64, 200})
{
    return sgl::BurstParams{
        .origin = {10.f, 20.f},
        .count = count,
        .speedMin = 0.f,
        .speedMax = 0.f,
        .angleMinDeg = 0.f,
        .angleMaxDeg = 0.f,
        .lifeMin = 1.f,
        .lifeMax = 1.f,
        .size = 4.f,
        .color = color,
        .gravity = {},
    };
}

} // namespace

TEST(ParticleSystemTest, emitRespectsCapacity)
{
    sgl::ParticleSystem system{2};
    sgl::Pcg32 rng{1};
    system.emit(makeBurst(5), rng);
    EXPECT_EQ(system.size(), 2u);
}

TEST(ParticleSystemTest, particlesDieAfterLifetime)
{
    sgl::ParticleSystem system{8};
    sgl::Pcg32 rng{2};
    auto params = makeBurst(3);
    params.lifeMin = 0.5f;
    params.lifeMax = 0.5f;
    system.emit(params, rng);
    ASSERT_EQ(system.size(), 3u);

    system.update(sgl::Seconds{0.49f});
    EXPECT_EQ(system.size(), 3u);

    system.update(sgl::Seconds{0.02f});
    EXPECT_EQ(system.size(), 0u);
}

TEST(ParticleSystemTest, alphaFadesLinearly)
{
    sgl::ParticleSystem system{1};
    sgl::Pcg32 rng{3};
    auto params = makeBurst(1, {10, 20, 30, 200});
    params.lifeMin = 1.f;
    params.lifeMax = 1.f;
    system.emit(params, rng);

    system.update(sgl::Seconds{0.5f});
    sgl::RenderQueue queue;
    system.render(queue, sgl::Layer::Overlay);
    ASSERT_EQ(queue.size(), 1u);
    const auto& cmd = std::get<sgl::RectCmd>(queue.sorted()[0].command);
    EXPECT_EQ(cmd.fill.a, 100);
    EXPECT_EQ(cmd.fill.r, 10);
    EXPECT_EQ(cmd.fill.g, 20);
    EXPECT_EQ(cmd.fill.b, 30);
}

TEST(ParticleSystemTest, renderPushesOneRectPerParticle)
{
    sgl::ParticleSystem system{16};
    sgl::Pcg32 rng{4};
    system.emit(makeBurst(7), rng);
    sgl::RenderQueue queue;
    system.render(queue, sgl::Layer::Actors);
    EXPECT_EQ(queue.size(), 7u);
    for(const sgl::Entry& entry: queue.sorted())
    {
        EXPECT_TRUE(std::holds_alternative<sgl::RectCmd>(entry.command));
        EXPECT_EQ(entry.key.layer, sgl::Layer::Actors);
    }
}

TEST(ParticleSystemTest, deterministicForSeed)
{
    auto run = [](std::uint64_t seed) {
        sgl::ParticleSystem system{32};
        sgl::Pcg32 rng{seed};
        sgl::BurstParams params{
            .origin = {0.f, 0.f},
            .count = 5,
            .speedMin = 10.f,
            .speedMax = 50.f,
            .angleMinDeg = 0.f,
            .angleMaxDeg = 360.f,
            .lifeMin = 2.f,
            .lifeMax = 2.f,
            .size = 3.f,
            .color = {255, 255, 255, 255},
            .gravity = {0.f, 100.f},
        };
        system.emit(params, rng);
        system.update(sgl::Seconds{0.25f});
        sgl::RenderQueue queue;
        system.render(queue, sgl::Layer::World);
        return queue;
    };

    auto a = run(99);
    auto b = run(99);
    auto c = run(100);
    ASSERT_EQ(a.size(), b.size());
    for(std::size_t i = 0; i < a.size(); ++i)
    {
        const auto& ra = std::get<sgl::RectCmd>(a.sorted()[i].command);
        const auto& rb = std::get<sgl::RectCmd>(b.sorted()[i].command);
        EXPECT_FLOAT_EQ(ra.rect.pos.x, rb.rect.pos.x);
        EXPECT_FLOAT_EQ(ra.rect.pos.y, rb.rect.pos.y);
        EXPECT_FLOAT_EQ(ra.rotationDeg, rb.rotationDeg);
    }
    const auto& ra0 = std::get<sgl::RectCmd>(a.sorted()[0].command);
    const auto& rc0 = std::get<sgl::RectCmd>(c.sorted()[0].command);
    EXPECT_TRUE(ra0.rect.pos.x != rc0.rect.pos.x || ra0.rect.pos.y != rc0.rect.pos.y);
}
