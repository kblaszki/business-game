#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

#include <eng/render/RendererI.hpp>
#include <eng/sfml/SfmlAssets.hpp>

#include <cstdint>

namespace eng
{
class RenderQueue;
}

namespace eng::sfml
{

class SfmlRenderer : public eng::RendererI
{
public:
    SfmlRenderer(sf::RenderTarget& target, const SfmlAssets& assets);

    void begin() override;
    void submit(const eng::RenderQueue& queue) override;
    void end() override;

    [[nodiscard]] std::uint32_t skippedCommands() const;

private:
    sf::RenderTarget& target_;
    const SfmlAssets& assets_;
    std::uint32_t skipped_{};
};

} // namespace eng::sfml
