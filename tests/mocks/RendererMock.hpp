#pragma once

#include <eng/render/RenderQueue.hpp>
#include <eng/render/RendererI.hpp>
#include <gmock/gmock.h>

namespace eng
{

class RendererMock : public RendererI
{
public:
    MOCK_METHOD(void, begin, (), (override));
    MOCK_METHOD(void, submit, (const RenderQueue&), (override));
    MOCK_METHOD(void, end, (), (override));
};

} // namespace eng
