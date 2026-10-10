# CLAUDE

All project rules for AI assistants live in **[AGENTS.md](AGENTS.md)**. Read it first and follow it; it is the single source of truth (shared with Codex and other tools).

Most important guardrails, repeated here for quick reference:

- `interfaces/` holds contracts (DBC, VSS, diagnostics). Change them only together with an ADR and a version bump.
- Never hand-edit CubeMX output (`Core/`, `Drivers/`, `.ioc`) or generated CAN code (`app/com/generated/`).
- Firmware layers call downward only: `functions` → `com` / `bsp` → HAL. No HAL calls in `functions/`.
- Every feature traces to requirement IDs in `docs/requirements/*.sdoc` (e.g. `SW-ZCS-021`); tests carry `@verifies <ID>`. Check with `python3 tools/req_check.py`.
- Feature work follows Spec Kit (`/speckit-*`) and lives in `specs/NNN-name/`.
