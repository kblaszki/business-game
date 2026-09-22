# Package cards and agent prompts

Read this after [SKILL.md](SKILL.md).

## Package card

The orchestrator fills one card per agent:

```
Goal: (one sentence)
Owned write paths: (src/..., tests/..., and the matching docs/ pages — always include docs/ when src/ is owned)
Forbidden paths: (everything else, especially siblings in this wave)
Locked signatures: (paste from contract)
Read first: (existing headers, contract, listed mvp/ or docs)
Verify: cmake --build --preset debug --target build_ut && ctest --preset debug
          (or “no code — skip build”)
```

Owned writes **must** include the matching `docs/` paths whenever the card owns `src/`. Parallelize two cards only when owned write paths do not overlap.

Typical v0.1 owned paths: `src/controllers/`, `src/managers/`, `src/screens/MenuScreen.*` / `GameScreen.*`, `src/entities/`, `tests/unit_tests/<area>/`. Matching docs: `docs/reference/architecture.md`, `docs/how-to/add-entity.md`, `docs/how-to/add-screen.md` — not FooI `screens-and-input.md`.

## Implement prompt

`model: cursor-grok-4.6-high-fast`, `subagent_type: generalPurpose`.

```
You own ONE work package. Implement it.

READ FIRST:
- CONTRACT / locked signatures (do not invent synonyms)
- SOURCE_PATHS (do not contradict the current tree)

OWNED WRITE PATHS (only these):
- ...

FORBIDDEN: do not edit other files. This tree is v0.1-arkanoid on main; do not copy FooI IScreen/ScreenStack/GameplayScreen from v0.2-fooi-base unless the contract says so.

DO:
- Implement the goal in C++23, #pragma once, no namespaces, CamelCase types
- List every new src/*.cpp in src/CMakeLists.txt (gameLib)
- Add windowless GoogleTest via add_unit_test in tests/unit_tests/CMakeLists.txt under tests/unit_tests/<area>/
- You must produce a docs/ edit if you change src/ (hard gate)
- Update docs whose related_code lists a file you changed; last_reviewed today
- New/removed src file or target → docs/reference/source-layout.md
- New type or new player-visible / loop behavior → update or add a docs/reference/ (or how-to) page; source-layout inventory alone is not enough
- New/removed/retitled doc → docs/index.md
- Do not git commit (the orchestrator commits after stitch)

VERIFY before you finish: VERIFY_COMMAND

GOAL:
- (one paragraph)

LOCKED SIGNATURES:
- (paste)
```

## Docs-only prompt (optional)

Use only when the user asked for documents, not code.

```
You own ONE document path. WRITE ONLY that file.

READ FIRST: contract + listed sources.
Do not implement C++. Do not edit siblings. Do not git commit.

Include: purpose, non-goals, sibling links, pitfalls.
If you use mermaid: PascalCase/camelCase node ids, no spaces;
quoted labels with punctuation; subgraph id [Label];
no style, colors, or click.
```

## Stitch checklist

- Build and tests (when the wave included code)
- Signature / name drift vs contract
- Two agents wrote the same path
- New `.cpp` missing from `gameLib`
- Docs `related_code` current; `last_reviewed` today; `source-layout.md` / `index.md` if structure changed
- Hard gate: `src/` in the wave diff without any `docs/` path → incomplete, no commit
- New behavior has a reference/how-to page, not only a source-layout row
- Contract / todos updated with locked decisions
- Docs hard gate or verify failed → no commit
- Verify passed → orchestrator committed owned paths + matching docs (record the hash)
- Dirty tree is only the next wave (or clean). Never staged `build/`
