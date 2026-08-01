/* Created by kblaszki */

#include "GameWindowManager.hpp"

GameWindowManager::GameWindowManager(WindowViewI& windowView)
    : windowView{windowView}
{
    applyLetterbox(windowView.getSize());
}

void GameWindowManager::handleEvent(const sf::Event& event)
{
    if(const auto* resized = event.getIf<sf::Event::Resized>())
    {
        applyLetterbox(resized->size);
    }
    // FocusLost / FocusGained reserved for future auto-pause.
}

void GameWindowManager::applyLetterbox(sf::Vector2u windowSize)
{
    if(0 == windowSize.x or 0 == windowSize.y)
    {
        return;
    }

    const float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    const float designRatio = DESIGN_SIZE.x / DESIGN_SIZE.y;

    sf::FloatRect viewport{{0.f, 0.f}, {1.f, 1.f}};
    if(windowRatio > designRatio)
    {
        const float width = designRatio / windowRatio;
        viewport.position.x = (1.f - width) / 2.f;
        viewport.size.x = width;
    }
    else if(windowRatio < designRatio)
    {
        const float height = windowRatio / designRatio;
        viewport.position.y = (1.f - height) / 2.f;
        viewport.size.y = height;
    }

    sf::View view{sf::FloatRect{{0.f, 0.f}, DESIGN_SIZE}};
    view.setViewport(viewport);
    windowView.setView(view);
}
