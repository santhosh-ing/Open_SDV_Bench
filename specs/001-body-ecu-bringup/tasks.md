# Tasks: Body ECU bring-up

**Input**: [spec.md](spec.md), [plan.md](plan.md)

## Phase 1: Setup

- [x] T001 Ratify the constitution from `docs/spec-inputs/00-constitution-input.md` (`.specify/memory/constitution.md`)
- [x] T002 Add app sources and include paths to `ecus/body-zone-stm32/CMakeLists.txt`
- [x] T003 Host test project with Unity in `ecus/body-zone-stm32/tests/CMakeLists.txt`

## Phase 2: Foundational

- [x] T004 [P] `app/bsp/bsp_tick.c/.h` — millisecond time base
- [x] T005 [P] `app/bsp/bsp_led.c/.h` — LED IDs, set/toggle/set_all
- [x] T006 `app/sched/sched.c/.h` — 1/10/100 ms slots, catch-up, debugger counters (FR-003, FR-006)
- [x] T007 `tests/unit/test_sched.c` — periods, no drift under late polling, wrap (`@verifies SW-ZCS-023`)
- [x] T008 `app/app.c/.h` and `main.c` USER CODE calls (FR-008)

## Phase 3: User Story 1 — started and alive (P1) 🎯 MVP

- [x] T009 `app/functions/ecu_state.c/.h` — Init/Normal, lamp test, heartbeat (FR-001, FR-002, FR-007)
- [x] T010 `tests/fakes/fake_bsp_led.c` + `tests/unit/test_ecu_state.c` — lamp test 1000 ms, heartbeat 1 Hz/50 % (`@verifies SW-ZCS-022`)

## Phase 4: User Story 2 — button detected once (P2)

- [x] T011 `app/bsp/debounce.c/.h` — 5 × 10 ms stable samples, one event per press (FR-004)
- [x] T012 `app/bsp/bsp_button.c/.h` — PA0 sampling, wired into the 10 ms slot
- [x] T013 `tests/unit/test_debounce.c` — bounce, short press, hold, held at reset, 10 presses
- [x] T014 Orange toggle on press in Normal, ignored during lamp test (FR-005) + tests

## Phase 5: User Story 3 — debugger counters (P3)

- [x] T015 `volatile` globals `g_sched_uptime_ms`, `g_sched_task100_count`, `g_ecu_state` (FR-006, FR-007)

## Phase 6: Polish

- [x] T016 CI workflow `.github/workflows/firmware.yml`: host tests + target build (SC-004, SC-005)
- [x] T017 `python3 tools/req_check.py` passes; devlog entry

## Phase 7: Flash and observe (maintainer, on the board)

See [quickstart.md](quickstart.md) for the exact steps.

- [ ] T018 Build and flash the Debug preset; board starts
- [ ] T019 SC-001: reset → all 4 LEDs on for ~1 s → all off → blue blinks 1 Hz
- [ ] T020 SC-002: 10 button presses → orange toggles exactly 10 times (orange is off again after an even count)
- [ ] T021 Edge case: hold the button through reset → no toggle until released and pressed again
- [ ] T022 SC-003: live expressions `g_sched_uptime_ms` ≈ 60000 after 60 s by stopwatch (± 600); `g_sched_task100_count` ≈ uptime / 100; `g_ecu_state` = `ECU_STATE_NORMAL`
- [ ] T023 Record the result in `docs/devlog.md`; set the spec status to Verified
