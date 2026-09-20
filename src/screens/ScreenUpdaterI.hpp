/* Created by kblaszki */
#pragma once

#include <memory>

class ScreenI;

class ScreenUpdaterI
{
public:
    virtual ~ScreenUpdaterI() = default;

    virtual void requestPush(std::unique_ptr<ScreenI> screen) = 0;
    virtual void requestPop() = 0;
    virtual void requestReplace(std::unique_ptr<ScreenI> screen) = 0;
    virtual void requestClose() = 0;
    virtual void requestPauseOverlay() = 0;
};
