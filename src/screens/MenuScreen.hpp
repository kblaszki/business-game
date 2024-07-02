/* Created by kblaszki */
#pragma once

#include "ScreenI.hpp"
#include "ScreenUpdaterI.hpp"

#include <entities/EntityI.hpp>
#include <managers/EventManagers.hpp>
#include <window/ScreenRendererI.hpp>

#include <memory>
#include <vector>

class MenuScreen : public ScreenI
{
public:
    MenuScreen(EventManagers& eventManagers, ScreenRendererI& screenRenderer, ScreenUpdaterI& screenUpdater);

    void update() override;
    void display() override;

private:
    std::vector<std::unique_ptr<EntityI>> entities;
    ScreenRendererI& screenRenderer;
};