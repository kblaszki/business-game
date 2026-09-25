---
title: Tetris sim
diataxis: reference
audience: [ai, human]
related_code:
  - games/tetris/sim/include/tetris/sim/Types.hpp
  - games/tetris/sim/include/tetris/sim/Shapes.hpp
  - games/tetris/sim/include/tetris/sim/Srs.hpp
  - games/tetris/sim/include/tetris/sim/Grid.hpp
  - games/tetris/sim/include/tetris/sim/Piece.hpp
  - games/tetris/sim/src/Grid.cpp
  - games/tetris/sim/src/Piece.cpp
  - games/tetris/sim/CMakeLists.txt
  - tests/tetris/sim/GridTest.cpp
  - tests/tetris/sim/SrsTest.cpp
  - tests/tetris/sim/CMakeLists.txt
related_docs:
  - engine-core.md
  - source-layout.md
  - ../explanation/architecture.md
keywords: [tetris, sim, Grid, Piece, SRS, kicks, shape, Rotation, ClearedRows, spawnPiece]
last_reviewed: 2026-09-25
---

# Tetris sim

Headless Tetris rules in `namespace sgl::tetris`. Static library target `tetris_sim` links `PUBLIC` `sgl_core` only. No SFML. Include as `<tetris/sim/X.hpp>`.

Coordinates: **+y up**, row 0 is the bottom of the 10×40 grid (`kVisibleHeight` 20).

## Types (`Types.hpp`)

| Name | Role |
|------|------|
| `PieceType` | `I`, `O`, `T`, `S`, `Z`, `J`, `L` |
| `Rotation` | `Spawn`, `Right`, `Reverse`, `Left` (0, R, 2, L) |
| `Turn` | `Cw`, `Ccw` |
| `Coord` | `x`/`y` with `<=>` and `operator+` |
| `Cells` | `std::array<Coord, 4>` |
| `Tile` | `std::optional<PieceType>` |

## Shapes (`Shapes.hpp`)

`boxSize`: I→4, O→2, others→3. `shape(type, rotation)` returns cells in the local `n×n` box. Spawn offsets match the guideline boxes; CW rotation is `(x, y) → (y, n - 1 - x)`. O cells are fixed for every rotation. Compile-time checks require four distinct cells per state and that four CW turns return the spawn set.

## Grid (`Grid.hpp`)

| API | Behavior |
|-----|----------|
| `inside` | `[0, 10) × [0, 40)` |
| `occupied` | Outside counts as occupied; else tile present |
| `fits` | All four cells free |
| `lock` | Writes `PieceType` into cells (precondition: `fits`) |
| `clearFullRows` | Records full-row indices, `std::ranges::remove_if` full rows (`all_of` tiles), fills freed top rows empty |
| `rows` | `span` of 40 rows |

`ClearedRows` holds up to four original indices and `count`.

## Piece and SRS (`Piece.hpp`, `Srs.hpp`)

`Piece` is `{type, rotation, origin}`. `cells` is origin plus `shape`. `spawnPiece` uses column 3 and places the box top at row 21. `tryShift` / `tryRotate` / `dropToFloor` are pure queries on a `Grid`.

`tryRotate` uses the five SRS kick tests from `kicks` (y-up tables for JLSTZ and I). O has no kicks; rotation only updates the enum (cells unchanged).

## Tests

`GridTest` covers fit/occupy/lock/clear. `SrsTest` covers spawn cells, CW identity, O immobility, JLSTZ and I kicks 1–5, all-kicks-blocked failure, and hard drop onto a stack.
