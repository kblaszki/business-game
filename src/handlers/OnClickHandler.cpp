/* Created by kblaszki */

#include "OnClickHandler.hpp"

OnClickHandler::OnClickHandler(MouseManagerI& mouseManager, const sf::Mouse::Button button)
    : unregisterHandler{mouseManager.registerButtonHandler(
          button,
          [this](const MouseButtonStatus status, const sf::Event::MouseButtonPressed& event) {
              if(MouseButtonStatus::Pressed == status)
              {
                  if(isHover(event.position.x, event.position.y))
                  {
                      isClicked = true;
                      onClick();
                  }
              }
              else
              {
                  if(isClicked)
                  {
                      isClicked = false;
                      onUnClick(isHover(event.position.x, event.position.y));
                  }
              }
          })}
    , button{button}
{
}
