# Requirements

Requirements are managed as text files with [StrictDoc](https://strictdoc.readthedocs.io/) (decision: [ADR 0004](../adr/0004-requirements-management-with-strictdoc.md)). Each requirement is an object with a stable ID, attributes and links, versioned in git and checked by CI on every pull request.

## Hierarchy

Each level refines the one above and is verified by its own test level (V-model):

| File | Level | ID scheme | Verified by |
|---|---|---|---|
| `00-stakeholder-needs.sdoc` | Stakeholder needs | `STK-NNN` | Demonstration on the bench |
| `10-system-requirements.sdoc` | System (vehicle level) | `SYS-<AREA>-NNN` | System tests on the bench |
| `20-function-requirements.sdoc` | Logical functions, node-independent | `FUN-<AREA>-NNN` | Integration tests (HPC + virtual twins) |
| `30-zc-stm32-requirements.sdoc` | Component: body zone controller | `SW-ZCS-NNN`, `HW-ZCS-NNN` | Host unit tests, tests on the board |

Later component files follow the same pattern: `31-hpc` (`SW-HPC`), `32-zc-arduino` (`SW-ZCA`), `33-adas` (`SW-ADAS`), `34-cloud` (`SW-CLOUD`).

Areas used so far: `LIGHT` (exterior lighting), `SAFE` (robustness, fail-safe), `DIAG` (diagnostics), `ARCH` (architecture).

**Link rules** (enforced by `tools/req_check.py`):

- `SYS` → parent `STK`; `FUN` → parent `SYS`; `SW`/`HW` → parent `FUN` (constraints may trace directly to `SYS`).
- Every requirement below `STK` has at least one parent.
- Function requirements are node-independent; allocation to a node happens at component level (`ALLOCATED_TO`). A function without component children is reported as "not yet refined", which makes open allocation decisions visible.

## Attributes

Defined once in the shared grammar `bench.sgra`:

| Field | Required | Values / meaning |
|---|---|---|
| `UID` | yes | Stable ID. Never reused, never renamed. |
| `STATUS` | yes | `Draft` → `Agreed` → `Implemented` → `Verified`, or `Obsolete` |
| `KIND` | yes | `Functional`, `Non-functional`, `Constraint` |
| `ALLOCATED_TO` | component level | Node: `ZC-STM32`, `HPC`, `ZC-Arduino`, `ADAS`, `Cloud` |
| `VERIFICATION` | yes | `Test`, `Demonstration`, `Inspection`, `Analysis` |
| `MILESTONE` | no | `M1` … `M8` |
| `RELEVANCE` | no | `Safety`, `Security` (both possible) |
| `LEGACY_ID` | no | Former ID, for requirements migrated from the old table |
| `TITLE`, `STATEMENT` | yes | Short name and the requirement itself |
| `RATIONALE`, `COMMENT` | no | Why it exists; open questions |

## Writing a requirement

Use EARS patterns, so every statement is testable:

| Pattern | Template | Example |
|---|---|---|
| Ubiquitous | The `<system>` shall `<response>`. | The vehicle shall switch its low beam on and off on request. |
| Event-driven | When `<trigger>`, the `<system>` shall `<response>`. | When the driver taps a direction request for less than 0.5 s, … |
| State-driven | While `<state>`, the `<system>` shall `<response>`. | While the hazard warning is requested, … |
| Unwanted behaviour | If `<condition>`, then the `<system>` shall `<response>`. | If no valid light command has been received for 500 ms or longer, … |

One requirement, one "shall". Put numbers in the statement (timing, tolerances), the reason in `RATIONALE`, and open questions in `COMMENT`.

## Change process

1. **Change by pull request only.** Edit the `.sdoc` files (by hand, or in the StrictDoc web editor with `strictdoc server docs/requirements`) and open a PR. The PR template asks for the reason and the impact.
2. **CI validates** syntax, unique IDs and links (StrictDoc), and the project rules above (`tools/req_check.py`).
3. **Suspect links:** if a requirement's title or statement changes, CI lists its children as warnings. Review each child and update it, or confirm it is still valid in the PR.
4. **Never delete.** Set `STATUS: Obsolete` instead, so old test reports and commits stay traceable. Active requirements may not trace to obsolete ones.
5. **Status follows evidence.** A requirement becomes `Implemented` or `Verified` only when at least one test carries `@verifies <ID>`.
6. **Baselines** are git tags on `main`, e.g. `req-baseline-M1`, created when a milestone's requirements are agreed.

## Linking tests and code

Tests name what they verify with a marker in a comment, anywhere in the test file:

```c
/* @verifies SW-ZCS-021 */
void test_command_timeout_enters_degraded(void) { … }
```

```python
def test_hazard_overrides_indicator():  # @verifies FUN-LIGHT-002
    ...
```

Spec Kit specs list the IDs they implement under "Traces to", and commit messages reference them, e.g. `spec-002: command timeout (SW-ZCS-021)`.

## Local commands

```bash
pip install strictdoc==0.30.2

# Validate and export (HTML + ReqIF) into build/req
strictdoc export docs/requirements --formats html,reqif-sdoc --output-dir build/req

# Project rules, traceability matrix and suspect-link review
python3 tools/req_check.py --report-html build/req/html/traceability.html
python3 tools/req_check.py --impact origin/main

# Browse and edit in the browser
strictdoc server docs/requirements
```

## Published report

CI builds the StrictDoc HTML export, the traceability matrix (`traceability.html`) and a ReqIF file on every run, available as the `requirements-report` artifact. On `main` the project website publishes the same report at [/requirements/](https://santhosh-ing.github.io/Open_SDV_Bench/requirements/) (`.github/workflows/site.yml`, spec 003).
