#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <sgl/core/Vec2.hpp>
#include <sgl/loop/PlatformI.hpp>
#include <sgl/sfml/SfmlAssets.hpp>
#include <sgl/sfml/SfmlRenderer.hpp>
#include <string>

namespace sgl::sfml
{

class SfmlPlatform : public sgl::PlatformI
{
public:
    SfmlPlatform(sgl::Vec2u designSize, std::string title);

    bool isOpen() const override;
    void close() override;
    std::optional<sgl::InputEvent> poll() override;
    sgl::RendererI& renderer() override;

    [[nodiscard]] SfmlAssets& assets();
    [[nodiscard]] const SfmlAssets& assets() const;

private:
    void applyLetterbox(sf::Vector2u windowSize);

    // window first so assets/renderer die while the GL context still lives.
    sf::RenderWindow window_;
    SfmlAssets assets_;
    SfmlRenderer renderer_;
    sgl::Vec2u designSize_{};
    sf::View view_{};
};

} // namespace sgl::sfml
