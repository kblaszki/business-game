#pragma once

#include <eng/input/InputEvent.hpp>
#include <eng/render/RendererI.hpp>
#include <optional>

namespace eng
{

class PlatformI
{
public:
    virtual ~PlatformI() = default;

    virtual bool isOpen() const = 0;
    virtual void close() = 0;
    virtual std::optional<InputEvent> poll() = 0;
    virtual RendererI& renderer() = 0;
};

} // namespace eng
