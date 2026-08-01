# Architecture and stack review (MVP audit)

One-off audit of business-game, written 2026-08-01. Standalone artifact — the living project documentation is in `docs/`.

## Verdict

The architecture is fundamentally sound for an SFML game — interface segregation, dependency injection, and testability are already in place and should be kept. The stack is outdated in two places: SFML 2.6.1 (current line is 3.1.0) and GoogleTest 1.14 (current 1.17), plus a deprecated CMake FetchContent pattern. Migration to SFML 3.1 is recommended and its cost is contained because SFML touches only a handful of files. Four correctness issues need fixing regardless of the migration: a dangling-handler bug in `Paddle`, no delta time in the game loop, per-instance font loading from a CWD-relative path, and per-event logging to stderr. A full rewrite is not warranted; the phased refactor below is.

## Documents

| Document | Contents |
|----------|----------|
| [stack-review.md](stack-review.md) | Version matrix, why migrate SFML, build-system fix checklist |
| [architecture-review.md](architecture-review.md) | What to keep, 8 issues by severity, each with a fix checklist |
| [sfml3-migration.md](sfml3-migration.md) | 11-step migration checklist with before/after code for this repo |
| [refactoring-roadmap.md](refactoring-roadmap.md) | Master plan: phases 0–5 with exit criteria and sizing |

## Reading order

1. This page (verdict)
2. [refactoring-roadmap.md](refactoring-roadmap.md) — the "what to do" summary
3. The three detail documents as needed per phase

## Status at a glance

| Area | State |
|------|-------|
| Architecture foundations (ISP, DI, tests) | Good — keep |
| SFML version | 2.6.1 → migrate to 3.1.0 (phase 2) |
| Build system | 3 fixes needed (phase 1) |
| Correctness | 1 real bug + 2 traps (phase 3) |
| Game-engine fundamentals (dt, resources, scene stack) | Missing (phases 3–4) |
| Dead/WIP code (Player, Ball, EntitiesManagerI) | Decision required (phase 4) |
