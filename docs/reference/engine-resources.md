---
title: Engine resource cache
diataxis: reference
audience: [ai, human]
related_code:
  - engine/resources/include/sgl/resources/ResourceCache.hpp
  - engine/resources/include/sgl/resources/ResourceError.hpp
  - engine/resources/CMakeLists.txt
  - engine/render/include/sgl/render/AssetIds.hpp
  - tests/engine/resources/ResourceCacheTest.cpp
related_docs:
  - source-layout.md
  - engine-render.md
  - ../explanation/architecture.md
keywords: [eng, ResourceCache, ResourceError, TextureId, FontId, load, get, find]
last_reviewed: 2026-09-25
---

# Engine resource cache

Header-only keyed cache in `namespace sgl`, included as `<sgl/resources/X.hpp>`. Target `sgl_resources` is an INTERFACE library that INTERFACE-links `sgl_core`. No SFML dependency.

Asset identity types `TextureId` / `FontId` come from `<sgl/render/AssetIds.hpp>`; this module does not redefine them. Typical usage is `ResourceCache<TextureId, T>` / `ResourceCache<FontId, T>` with a caller-supplied loader.

## ResourceError

`enum class ResourceError { NotFound, DecodeFailed, Unsupported }`.

`describe(error)` returns a stable `std::string_view` label (`"NotFound"`, `"DecodeFailed"`, `"Unsupported"`).

## ResourceCache\<Id, T\>

Storage is a `std::vector<std::unique_ptr<T>>` plus a parallel `std::vector<std::string>` of keys. `Id::id` is `index + 1`; `0` is invalid.

| API | Behavior |
|-----|----------|
| `load(key, loader)` | If `key` exists, returns the existing `Id` and does not call `loader`. Otherwise invokes `loader`; on success stores `T` and returns a new `Id`; on error returns `ResourceError` and leaves `size()` unchanged |
| `get(id)` | Pointer to stored `T`, or `nullptr` for invalid / out-of-range `id`. Pointers remain valid across later successful inserts |
| `find(key)` | `std::optional<Id>` for an existing key |
| `size()` | Number of stored resources |

`Loader` must satisfy `ResourceLoader`: invocable as `Loader&`, with `std::invoke_result_t<Loader&>` equal to `std::expected<T, ResourceError>`.
