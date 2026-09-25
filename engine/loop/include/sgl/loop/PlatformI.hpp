#pragma once

#include <optional>
#include <sgl/input/InputEvent.hpp>
#include <sgl/render/RendererI.hpp>

namespace sgl
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

} // namespace sgl
