/* Created by kblaszki */

#include "OnClickHandler.hpp"

OnClickHandler::OnClickHandler(MouseManagerI& mouseManager, const sf::Mouse::Button button)
    : unregisterHandler{mouseManager.registerButtonHandler(
          button,
          [this](const MouseButtonStatus status, const sf::Event::MouseButtonEvent& event) {
              if(MouseButtonStatus::Pressed == status)
              {
                  if(isHover(event.x, event.y))
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
                      onUnClick(isHover(event.x, event.y));
                  }
              }
          })}
    , button{button}
{
}

OnClickHandler::~OnClickHandler()
{
    unregisterHandler();
}
