#pragma once

#include <eng/core/Handle.hpp>

namespace eng
{

struct TextureTag
{
};

struct FontTag
{
};

using TextureId = Handle<TextureTag>;
using FontId = Handle<FontTag>;

} // namespace eng
