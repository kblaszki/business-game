/* Created by kblaszki */

#include "OnHoverHandler.hpp"

OnHoverHandler::OnHoverHandler(MouseManagerI& mouseManager)
    : unregisterHandler{mouseManager.registerMoveHandler([this](const sf::Event::MouseMoveEvent& event) {
        if(isHover(event.x, event.y))
        {
            if(not isHovered)
            {
                isHovered = true;
                onHover();
            }
        }
        else
        {
            if(isHovered)
            {
                isHovered = false;
                onHoverOut();
            }
        }
    })}
{
}

OnHoverHandler::~OnHoverHandler()
{
    unregisterHandler();
}
