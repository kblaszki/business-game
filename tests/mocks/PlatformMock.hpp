#pragma once

#include <gmock/gmock.h>
#include <sgl/loop/PlatformI.hpp>

class PlatformMock : public sgl::PlatformI
{
public:
    MOCK_METHOD(bool, isOpen, (), (const, override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD((std::optional<sgl::InputEvent>), poll, (), (override));
    MOCK_METHOD(sgl::RendererI&, renderer, (), (override));
};
