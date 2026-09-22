# Examples

## Code: v0.1 areas as sequential waves

When implementing on this tree, packages follow **v0.1 directories**, not FooI `IScreen` / `ScreenStack` / `GameplayScreen`.

| Wave | Package | Owned writes (typical) |
|------|---------|------------------------|
| 1 | Controllers | `src/controllers/` (`GameController`, `EventController`, `ScreenController`), `tests/unit_tests/controllers/`, `docs/reference/architecture.md` |
| 2 | Managers | `src/managers/`, `tests/unit_tests/managers/`, `docs/reference/event-flow.md` / `docs/how-to/add-event-manager.md` |
| 3 | Screens | `src/screens/MenuScreen.*` / `GameScreen.*`, `tests/unit_tests/screens/`, `docs/how-to/add-screen.md` |
| 4 | Entities | `src/entities/`, `tests/unit_tests/entities/`, `docs/how-to/add-entity.md` |
| 5 | Handlers / resources | `src/handlers/` or `src/resources/`, matching `tests/unit_tests/<area>/`, matching how-to/reference |
| 6 | Docs leftover sweep | leftover `docs/**` only (`related_code`, `docs/index.md`) |

Within a wave, split only if paths stay disjoint (e.g. wave 3: one agent `MenuScreen`, one agent `GameScreen` **if** they do not both edit `ScreenController.cpp`).

After each of waves 1–5:

1. Same-change Diátaxis docs for every touched file. New loop / screen / entity behavior updates the matching reference or how-to page, not only `source-layout.md`.
2. Hard gate: the wave diff must include a `docs/` path. If not, write the docs before commit.
3. Debug `build_ut` + `ctest --preset debug`.
4. One orchestrator commit (owned paths + matching docs). Do not commit if the docs gate or verify failed.

Wave 6 is a leftover-docs audit, not the first time docs are written. Commit that sweep if it changes files.

Locked names stay those in this tree: `GameController`, `ScreenController`, `EventController`, `MenuScreen`, `GameScreen`, `EntityI`, `ResourceManager`, `ScreenUpdaterI`.

## Docs-only: `mvp/` snapshots

`mvp/stack-review.md` and `mvp/refactoring-roadmap.md` are **historical**. Do not rewrite them as current toolchain facts. Use this docs-only fan-out only when the user asked for a written plan, not the implementation. Orchestrator commits after stitch if verify (links / contract lock) passes.
