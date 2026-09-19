# Examples

## Code: engine slice as sequential waves

When the user wants the playable slice **implemented**, treat [mvp/09-rollout.md](../../../mvp/09-rollout.md) as the package list. Phases depend on each other — **one wave after the previous verifies and commits**, not nine parallel code writers.

| Wave | Package | Owned writes (typical) |
|------|---------|------------------------|
| 1 | Clock + `IScreen` | `src/Game.*`, `src/IScreen.hpp`, tests for timestep, matching `docs/` |
| 2 | `ScreenStack` | `src/ScreenStack.*`, stack tests (fakes, no window), matching `docs/` |
| 3 | `MainMenuScreen` + `GameplayScreen` | those `src/*Screen.*` + transition tests, matching `docs/` |
| 4 | `Action` + `InputMapper` + FocusLost | `src/Action.hpp`, `src/InputMapper.*`, `src/Game.cpp` consume path, matching `docs/` |
| 5 | `PauseScreen` overlay | `src/PauseScreen.*`, pause-blocks-ticks test, matching `docs/` |
| 6 | `World` / `GameObject` / `LevelId` | those sources + `makeWorld` + sandbox descriptor, matching `docs/` |
| 7 | Docs leftover sweep | leftover `docs/**` only (`related_code`, `docs/index.md`) |

Within a wave, split only if paths stay disjoint (e.g. wave 3: one agent menu, one agent gameplay **if** they do not both edit `ScreenStack.cpp`).

After each of waves 1–6:

1. Same-change Diátaxis docs for every touched file (`related_code`, `source-layout.md` if a file/target was added).
2. Debug `build_ut` + `ctest --preset debug`.
3. One orchestrator commit (owned paths + matching docs). Do not commit if verify failed.

Wave 7 is a leftover-docs audit, not the first time docs are written. Commit that sweep if it changes files.

Locked names stay those in [mvp/README.md](../../../mvp/README.md).

## Docs-only: `mvp/` book

Same process, packages are files: contract [mvp/README.md](../../../mvp/README.md), then nine Grok 4.6 writers for [01](../../../mvp/01-architecture.md)–[09](../../../mvp/09-rollout.md), then stitch (see that README’s locked-decisions section). Orchestrator commits after stitch if verify (links / contract lock) passes. Use this only when the user asked for a written plan, not the implementation.
