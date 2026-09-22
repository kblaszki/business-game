---
name: update-docs
description: >-
  Audits and updates docs/ (Diataxis) after code changes so documentation stays accurate.
  Use when code changed and docs may be stale, when adding files/targets, or when asked to
  refresh, verify, or sync the documentation.
---

# Update docs

Keep `docs/` in sync with the code. Docs are the source of truth; each file has front-matter with `related_code`, `related_docs`, `keywords`, `last_reviewed`.

## Workflow

```
- [ ] Get the list of changed code files (e.g. git diff --name-only)
- [ ] Find docs whose front-matter related_code lists any changed file
- [ ] Update the content of those docs to match the code
- [ ] Set last_reviewed to today's date in each doc you edited
- [ ] Handle structural changes (new/removed file, target, directory)
- [ ] Verify links and related_code paths resolve
- [ ] Refresh docs/index.md if documents were added/removed/retitled
```

## Steps

1. **Changed files**: `git diff --name-only` (or the set you just edited).

2. **Map to docs**: search `docs/` for each changed path inside `related_code`. Update every matching doc so its statements match the new code. Then bump `last_reviewed` to today.

3. **Structural changes**:
   - New/removed/renamed `src/**` file or CMake target → update `docs/reference/source-layout.md`.
   - New event routing, interface, or loop change → update `docs/reference/` (`architecture.md`, `interfaces.md`, `event-flow.md`).
   - New workflow → update or add a `docs/how-to/` page.
   - Any added/removed/retitled doc → update the table in `docs/index.md`.

4. **Verify**: relative links resolve; every `related_code` path exists; facts (C++20, presets, targets, binary path) match `CMakeLists.txt`, `src/CMakeLists.txt`, `CMakePresets.json`.

## Front-matter shape

```yaml
---
title: ...
diataxis: tutorial | how-to | reference | explanation
audience: [ai, human]
related_code: [ ... source paths ... ]
related_docs: [ ... relative doc paths ... ]
keywords: [ ... ]
last_reviewed: YYYY-MM-DD
---
```

Keep this schema identical across docs so search stays reliable.
