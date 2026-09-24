#pragma once

#include <eng/loop/PlatformI.hpp>
#include <gmock/gmock.h>

class PlatformMock : public eng::PlatformI
{
public:
    MOCK_METHOD(bool, isOpen, (), (const, override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD((std::optional<eng::InputEvent>), poll, (), (override));
    MOCK_METHOD(eng::RendererI&, renderer, (), (override));
};
