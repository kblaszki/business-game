#include <eng/sfml/SfmlRenderer.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <eng/render/DrawCommand.hpp>
#include <eng/render/RenderQueue.hpp>

#include <cmath>
#include <type_traits>
#include <variant>

namespace eng::sfml
{
namespace
{

sf::Color toSfColor(eng::Color color)
{
    return sf::Color{color.r, color.g, color.b, color.a};
}

void setCenterOrigin(sf::Transformable& transformable, const sf::FloatRect& bounds)
{
    transformable.setOrigin({bounds.position.x + bounds.size.x * 0.5f, bounds.position.y + bounds.size.y * 0.5f});
}

} // namespace

SfmlRenderer::SfmlRenderer(sf::RenderTarget& target, const SfmlAssets& assets)
    : target_{target}
    , assets_{assets}
{
}

void SfmlRenderer::begin()
{
    skipped_ = 0;
    target_.clear();
}

void SfmlRenderer::submit(const eng::RenderQueue& queue)
{
    for(const eng::Entry& entry : const_cast<eng::RenderQueue&>(queue).sorted())
    {
        std::visit(
            [this](const auto& cmd)
            {
                using T = std::decay_t<decltype(cmd)>;
                if constexpr(std::is_same_v<T, eng::SpriteCmd>)
                {
                    const sf::Texture* texture = assets_.get(cmd.texture);
                    if(texture == nullptr)
                    {
                        ++skipped_;
                        return;
                    }

                    sf::Sprite sprite{*texture};
                    const auto left = static_cast<int>(std::lround(cmd.source.pos.x));
                    const auto top = static_cast<int>(std::lround(cmd.source.pos.y));
                    const auto width = static_cast<int>(std::lround(cmd.source.size.x));
                    const auto height = static_cast<int>(std::lround(cmd.source.size.y));
                    sprite.setTextureRect(sf::IntRect({left, top}, {width, height}));
                    sprite.setPosition({cmd.position.x, cmd.position.y});
                    sprite.setScale({cmd.scale.x, cmd.scale.y});
                    sprite.setColor(toSfColor(cmd.tint));
                    target_.draw(sprite);
                }
                else if constexpr(std::is_same_v<T, eng::RectCmd>)
                {
                    sf::RectangleShape shape{{cmd.rect.size.x, cmd.rect.size.y}};
                    shape.setPosition({cmd.rect.pos.x, cmd.rect.pos.y});
                    shape.setFillColor(toSfColor(cmd.fill));
                    if(cmd.outlineThickness != 0.f)
                    {
                        shape.setOutlineThickness(cmd.outlineThickness);
                        shape.setOutlineColor(toSfColor(cmd.outline));
                    }
                    target_.draw(shape);
                }
                else if constexpr(std::is_same_v<T, eng::TextCmd>)
                {
                    const sf::Font* font = assets_.get(cmd.font);
                    if(font == nullptr)
                    {
                        ++skipped_;
                        return;
                    }

                    sf::Text text{*font, cmd.text, cmd.size};
                    text.setFillColor(toSfColor(cmd.color));
                    if(cmd.anchor == eng::Anchor::Center)
                    {
                        setCenterOrigin(text, text.getLocalBounds());
                    }
                    text.setPosition({cmd.position.x, cmd.position.y});
                    target_.draw(text);
                }
            },
            entry.command);
    }
}

void SfmlRenderer::end()
{
    if(auto* window = dynamic_cast<sf::RenderWindow*>(&target_))
    {
        window->display();
    }
}

std::uint32_t SfmlRenderer::skippedCommands() const
{
    return skipped_;
}

} // namespace eng::sfml
