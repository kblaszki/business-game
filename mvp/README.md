# Architecture and stack review (MVP audit)

One-off audit of business-game, written 2026-08-01. Standalone artifact — the living project documentation is in `docs/`.

## Verdict

The architecture is fundamentally sound for an SFML game — interface segregation, dependency injection, and testability are already in place and should be kept. Phases 1–3 are done (build modernization, SFML 3.1, RAII handlers + fixed timestep). Remaining gaps are game-engine fundamentals (resource cache, scene stack) and dead/WIP code decisions in phase 4. A full rewrite is not warranted; the phased refactor below is.

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
| SFML version | 3.1.0 (phase 2 done 2026-08-01) |
| Build system | Done (phase 1) |
| Correctness (RAII handlers, fixed dt, no event spam) | Done (phase 3, 2026-08-01) |
| Game-engine fundamentals (resources, scene stack) | Missing (phase 4) |
| Dead/WIP code (Player, Ball, EntitiesManagerI) | Decision required (phase 4) |
