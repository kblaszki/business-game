/* Created by kblaszki */
#pragma once

#include "MouseManagerI.hpp"

class MouseManager : public MouseManagerI
{
public:
    void handleEvent(const sf::Event& event) override;
    MoveUnRegisterer registerMoveHandler(MoveHandler&& handler) override;

private:
    ManagedList<MoveHandler> mouseMoveHandlers;
};