/* Created by kblaszki */

#include "OnHoverHandler.hpp"

OnHoverHandler::OnHoverHandler(MouseManagerI& mouseManager)
    : unregisterHandler{mouseManager.registerMoveHandler([this](const sf::Event::MouseMoved& event) {
        if(isHover(event.position.x, event.position.y))
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
