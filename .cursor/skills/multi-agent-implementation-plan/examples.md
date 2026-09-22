# Examples

## Code: grow the skeleton in sequential waves

This tree is `src/main.cpp` (SFML window) + `src/Example.*` (`gameLib`) + `example_test`. Do not start from FooI `IScreen` / `ScreenStack` / `GameplayScreen` unless the contract names them.

| Wave | Package | Owned writes (typical) |
|------|---------|------------------------|
| 1 | `Example` / `gameLib` types | `src/Example.*` (or a new sibling type), `tests/unit_tests/ExampleTest.cpp`, `docs/reference/source-layout.md` |
| 2 | Window / `main` | `src/main.cpp`, `docs/tutorials/getting-started.md` |
| 3 | Docs leftover sweep | leftover `docs/**` only (`related_code`, `docs/index.md`) |

Within a wave, split only if paths stay disjoint.

After each of waves 1–2:

1. Same-change Diátaxis docs for every touched file. New types update `source-layout.md` (and a new reference/how-to if behavior needs a home).
2. Hard gate: the wave diff must include a `docs/` path. If not, write the docs before commit.
3. Debug `build_ut` + `ctest --preset debug`.
4. One orchestrator commit (owned paths + matching docs). Do not commit if the docs gate or verify failed.

Wave 3 is a leftover-docs audit, not the first time docs are written. Commit that sweep if it changes files.

Locked names stay those in this tree: `Example`, `game`, `gameLib`. Engine names in [`mvp/`](../../../mvp/README.md) are notes, not current types.

## Docs-only: `mvp/` snapshots

`mvp/` is **historical**. Do not rewrite it as current source facts. Use this docs-only fan-out only when the user asked for a written plan, not the implementation. Orchestrator commits after stitch if verify (links / contract lock) passes.
