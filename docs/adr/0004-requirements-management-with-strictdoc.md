# ADR 0004: Requirements management with StrictDoc

## Status

Accepted

## Context

Requirements lived in one Markdown table (`docs/requirements/body-ecu-requirements.md`). That does not scale: a table row has no history of its own, links between requirements cannot be checked, parallel changes collide as merge conflicts, and nothing shows which requirements are affected when another one changes. The spec inputs had already drifted from the table (spec 002 referenced IDs with a different meaning).

The project needs requirements that:

- form a hierarchy (stakeholder → system → function → component) that matches the architecture views,
- are atomic objects with stable IDs and attributes (status, verification method, milestone, safety/security relevance),
- link to their parents and to the tests that verify them,
- live in git as text and are checked automatically on every change,
- can be exchanged with professional tools (ReqIF).

## Decision

- Requirements are written in StrictDoc's text format (`.sdoc`) in `docs/requirements/`, one file per hierarchy level and node, with one shared grammar (`bench.sgra`) defining the attributes.
- IDs follow `STK-NNN`, `SYS-<AREA>-NNN`, `FUN-<AREA>-NNN`, `SW|HW-<NODE>-NNN`. They are never reused; requirements are made Obsolete instead of deleted.
- StrictDoc validates syntax, unique IDs and links. `tools/req_check.py` enforces the project rules: parent one level up, no links to obsolete requirements, Implemented/Verified only with a test carrying `@verifies <ID>`. It also produces a traceability matrix and a suspect-link review for pull requests.
- CI exports HTML and ReqIF on every run; the report is published on GitHub Pages from `main`.
- StrictDoc is pinned (0.30.2), because its file format still evolves between versions.
- The seven requirements of the old table are migrated with their former IDs kept in `LEGACY_ID`.

Alternative considered: Sphinx-Needs, which is more flexible (custom metamodels, used in Eclipse S-CORE's docs-as-code tooling) but heavier to set up. StrictDoc's ReqIF import/export and browser editor fit a single-maintainer project with a professional ReqIF background better.

## Consequences

- `REQ-<AREA>-<NNN>` IDs are retired; AGENTS.md, the constitution input and the spec inputs reference the new IDs.
- Requirement changes are pull requests and get reviewed like code.
- Upgrading StrictDoc is a deliberate change (format migration), not an accident.
- Test code must carry `@verifies` markers for requirements to reach Implemented/Verified.
