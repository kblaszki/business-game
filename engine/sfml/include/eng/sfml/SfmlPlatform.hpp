#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <eng/core/Vec2.hpp>
#include <eng/loop/PlatformI.hpp>
#include <eng/sfml/SfmlAssets.hpp>
#include <eng/sfml/SfmlRenderer.hpp>

#include <string>

namespace eng::sfml
{

class SfmlPlatform : public eng::PlatformI
{
public:
    SfmlPlatform(eng::Vec2u designSize, std::string title);

    bool isOpen() const override;
    void close() override;
    std::optional<eng::InputEvent> poll() override;
    eng::RendererI& renderer() override;

    [[nodiscard]] SfmlAssets& assets();
    [[nodiscard]] const SfmlAssets& assets() const;

private:
    void applyLetterbox(sf::Vector2u windowSize);

    // window first so assets/renderer die while the GL context still lives.
    sf::RenderWindow window_;
    SfmlAssets assets_;
    SfmlRenderer renderer_;
    eng::Vec2u designSize_{};
    sf::View view_{};
};

} // namespace eng::sfml
