---
title: Engine particle fx
diataxis: reference
audience: [ai, human]
related_code:
  - engine/fx/CMakeLists.txt
  - engine/fx/include/sgl/fx/ParticleSystem.hpp
  - engine/fx/src/ParticleSystem.cpp
  - tests/engine/fx/ParticleSystemTest.cpp
  - tests/engine/fx/CMakeLists.txt
related_docs:
  - source-layout.md
  - engine-core.md
  - engine-render.md
  - ../explanation/architecture.md
keywords: [eng, sgl_fx, ParticleSystem, Particle, BurstParams, Pcg32]
last_reviewed: 2026-09-25
---

# Engine particle fx

Fixed-capacity CPU particles in `namespace sgl`, included as `<sgl/fx/X.hpp>`. Target `sgl_fx` is a STATIC library that PUBLIC-links `sgl_core` and `sgl_render`. No SFML dependency.

## Types

| Type | Fields |
|------|--------|
| `Particle` | `pos`, `vel`, `size`, `rotationDeg`, `spinDeg`, `life`, `maxLife`, `color` |
| `BurstParams` | `origin`, `count`, `speedMin`/`speedMax`, `angleMinDeg`/`angleMaxDeg`, `lifeMin`/`lifeMax`, `size`, `color`, `gravity` |

Angles are degrees; velocity uses `cos`/`sin` of the sampled angle. Life is remaining seconds; `maxLife` is the starting life for alpha fade.

## ParticleSystem

`ParticleSystem(capacity)` reserves once in the constructor. No further heap allocation in `emit` / `update` / `render` (overflow particles are dropped; dead particles are removed by swap-pop).

| Method | Behavior |
|--------|----------|
| `emit(params, rng)` | spawn up to remaining capacity using `Pcg32::uniformFloat`; copies `gravity` per particle |
| `update(dt)` | integrate velocity with that gravity, advance spin/life, remove when `life <= 0` |
| `render(queue, layer)` | one `RectCmd` per live particle; top-left is `pos - size/2`; alpha fades linearly as `color.a * (life / maxLife)` |
| `size()` | live particle count |

Randomness must come from `sgl::Pcg32` (see [engine-core.md](engine-core.md)).
