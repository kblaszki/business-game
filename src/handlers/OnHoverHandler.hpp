/* Created by kblaszki */
#pragma once

#include "IsHoverHandlerI.hpp"

#include <managers/MouseManagerI.hpp>

class OnHoverHandler : virtual protected IsHoverHandlerI
{
public:
    OnHoverHandler(MouseManagerI& mouseManager);
    virtual ~OnHoverHandler() = default;

    virtual void onHover() = 0;
    virtual void onHoverOut() = 0;

private:
    MouseManagerI::MoveUnRegisterer unregisterHandler;
    bool isHovered{false};
};
