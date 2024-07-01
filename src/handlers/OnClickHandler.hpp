/* Created by kblaszki */
#pragma once

#include "IsHoverHandlerI.hpp"

#include <managers/MouseManagerI.hpp>

#include <iostream>

class OnClickHandler : virtual protected IsHoverHandlerI
{
public:
    OnClickHandler(MouseManagerI& mouseManager, sf::Mouse::Button button);
    virtual ~OnClickHandler();

    virtual void onClick() = 0;
    virtual void onUnClick(bool isHovered) = 0;

private:
    MouseManagerI::ButtonUnRegisterer unregisterHandler;
    sf::Mouse::Button button;
    bool isClicked{false};
};