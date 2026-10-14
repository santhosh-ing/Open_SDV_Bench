# Spec Kit inputs

Reviewed input texts for the Spec Kit commands. They are kept in the repo as the "source" of each spec, so the reasoning behind every generated `specs/NNN-*/spec.md` stays traceable.

| File | Use with |
|---|---|
| `00-constitution-input.md` | `/speckit-constitution` |
| `01-body-ecu-bringup.md` | Part A → `/speckit-specify`, Part B → `/speckit-plan` |
| `02-body-ecu-can-lights.md` | Part A → `/speckit-specify`, Part B → `/speckit-plan` |
| `03-project-website.md` | Part A → `/speckit-specify`, Part B → `/speckit-plan` (project infrastructure, can run in parallel with M1) |

## Order of work

1. Review the three files and `interfaces/can/body.dbc` (see ADR 0002).
2. Spec Kit is already initialized in this repo (**v1.1.3**, Claude integration; skills in `.claude/skills/`). Only to refresh or add another agent:
   `uvx --from git+https://github.com/github/spec-kit.git@v1.1.3 specify init --here --force --integration claude`
3. `/speckit-constitution` + `00-constitution-input.md`, then check `.specify/memory/constitution.md`.
4. Spec 001: generate the real CubeMX project (Board Selector → STM32F407G-DISC1, toolchain CMake) into `ecus/body-zone-stm32/`, flash a plain blink, then run
   `/speckit-specify` → `/speckit-clarify` → `/speckit-plan` → `/speckit-tasks` → `/speckit-implement`, and verify on the board.
5. Spec 002: same flow.

## Check the CAN contract

```bash
pip install cantools
python3 -m cantools dump interfaces/can/body.dbc
```
