#pragma once

#include <sgl/core/Handle.hpp>

namespace sgl
{

struct TextureTag
{
};

struct FontTag
{
};

using TextureId = Handle<TextureTag>;
using FontId = Handle<FontTag>;

} // namespace sgl
