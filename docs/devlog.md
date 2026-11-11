# Devlog

Short, dated entries: what was done, what was learned, what's next. Newest at the top.

## 2026-11-10

- electronica Munich, edge lab LIVE! workshops: FRDM Automotive (NXP FRDM-A-S32K344) and LVGL on Riverdi's 5″ STM32U5 display. Also collected an NXP FRDM-MCXA156, an ST debug probe and two Renesas FPB-RA2E3 kits with PTX105R NFC reader, NFC cards and SG90 servo.
- Added to `docs/hardware/bom.md` as **candidate boards**. Candidate roles (ideas only, `parking-lot` issues): rear zone controller ZC02 (S32K344, CAN FD), LIN smart node (MCXA156), instrument cluster (Riverdi + LVGL), door / digital-key module (RA2E3 + NFC + servo).
- ADR 0003 and the website topology are unchanged: a board joins the topology only through its own ADR.
- **Next:** unchanged, spec 004 (joystick turn indication) on the STM32 body zone.

## 2026-11-01 (night)

- Spec 001 verified on the board: lamp test, 1 Hz heartbeat, 10 button presses = 10 orange toggles, button held through reset = no toggle. Debugger after 60 s by stopwatch: `g_sched_uptime_ms` and `g_sched_task100_count` within tolerance, `g_ecu_state` = Normal.
- SW-ZCS-023 (deterministic timing) set to Verified. SW-ZCS-022 stays Draft until its Degraded part (4 Hz heartbeat) is built in spec 002.
- **Next:** spec 004, joystick turn indication and hazard (`docs/spec-inputs/04-body-ecu-joystick-indication.md`).

## 2026-11-01 (evening)

- Joystick wired on paper: KY-023 on PA1 (X axis), PE7 (push = hazard), powered from 3V (PR #10).
- Requirements updated for ADR 0005/0006: tap = comfort blink, hold = indication while held, joystick push toggles local hazard, vehicle states Off / On / Update with a 500 ms fail-safe that keeps local indication working and switches the low beam on. New: STK-006, SYS-VEH-001, FUN-LIGHT-011, FUN-LIGHT-012, FUN-VEH-001, SW-ZCS-050…054, HW-ZCS-001; changed: FUN-SAFE-001, SW-ZCS-010, SW-ZCS-021.
- Contract v0.2 proposed (ADR 0007: `VEH_State` 0x100, direction requests removed from `BODY_LightCmd`). Logical view added as Mermaid, mirroring the Capella LAB.
- Working agreement: firmware is written by the maintainer in VS Code; AI assistants update requirements, ADRs and diagrams only, and code serves as reference.
- **Next:** review and accept ADR 0005–0007; edit `body.dbc` to v0.2.0; update spec input 002; flash and verify spec 001.

## 2026-11-01

- Started an Arcadia model in Capella 7.0.1 (one model; physical architecture first, then a logical architecture per node, linked to the hardware).
- Proposed ADR 0005 (turn indication fully local in the ZC-STM32, joystick on the zone) and ADR 0006 (vehicle states Off/On/Update managed by the CGW role on the HPC, dashboard as input, fail-safe after 5 missed cycles, hazard per the production-vehicle concept).
- Both conflict with ADR 0002 / `body.dbc` v0.1.0 and with FUN-SAFE-001, SW-ZCS-010 and SW-ZCS-021; requirements are unchanged until those are resolved.
- **Next:** decide the hazard input in fail-safe, the vehicle state cycle time and the new fail-safe behaviour; then update the requirements and the contract (new ADR, DBC version bump).

## 2026-10-14 (late)

- Constitution ratified (v1.0.0) from the reviewed input.
- Spec 001 body ECU bring-up implemented: 1 ms cooperative scheduler with 1/10/100 ms slots, board support for LEDs, button and time base, ECU states Init/Normal with a 1 s lamp test, 1 Hz heartbeat and a debounced button demo. Only on-board LEDs and button, no wiring.
- 16 host unit tests (Unity) for debounce, scheduler timing and lamp test/heartbeat; new `firmware` CI job runs them and builds the target firmware (7.5 KB flash, 1.6 KB RAM).
- **Next:** flash and observe on the board (`specs/001-body-ecu-bringup/quickstart.md`); then spec 002 (CAN light control, loopback first) while the transceiver order is on its way.

## 2026-10-14

- Spec input for a public project website (spec 003): Astro + Starlight on GitHub Pages, landing page modelled on rea.tools, built from the existing `docs/` and including the requirements report. Skeleton in M0, feature showcases after M2.
- **Next:** decide the Pages ownership question (website vs. requirements report), then `/speckit-specify` with `docs/spec-inputs/03-project-website.md`.
- Project website built (spec 003): Astro + Starlight in `site/`, generated from `docs/` at build time, landing page with topology and roadmap, requirements report under `/requirements/`. `site.yml` now owns GitHub Pages; `requirements.yml` only validates.
- **Next:** enable Pages (Settings → Pages → Source: GitHub Actions) and check the live site; then back to `/speckit-constitution` and spec 001.

## 2026-10-11

- Body zone firmware builds and flashes from VS Code: the missing ARM compiler bundle (`gnu-tools-for-stm32` 14.3.1) was installed, a blink test build used 6 116 B flash and 1 584 B RAM, and STM32CubeProgrammer wrote and verified it over ST-LINK.
- Added `docs/guides/`: a beginner-friendly explanation of the firmware workflow (CubeMX → CMake → compile/link → flash → boot) and a setup, build and flash how-to with troubleshooting.
- **Next:** remove the test blink from `main.c`; `/speckit-constitution`, then spec 001 (body ECU bring-up).

## 2026-10-10 (late)

- Requirements moved from one Markdown table to a StrictDoc hierarchy (STK → SYS → FUN → SW-ZCS), 32 requirements, the 7 old ones migrated with their former IDs (ADR 0004).
- CI now validates requirements, enforces the link rules, reports suspect links on PRs and builds a traceability matrix plus ReqIF export.
- **Next:** enable GitHub Pages for the report; then `/speckit-constitution` and the STM32 blink.

## 2026-10-10 (evening)

- Target architecture agreed: central HPC star with CAN to the STM32 body zone controller, Ethernet to the Jetson and the Arduino sensor zone, and a cloud tier (OpenSDV.Cloud) over IP (ADR 0003).
- Network/deployment view added as an editable draw.io diagram; README rewritten around the SDV principles the bench proves.
- **Next:** unchanged — `/speckit-constitution`, then CubeMX project and a plain blink on the STM32.

## 2026-10-10

- Repo aligned with the agreed structure (contracts, docs views, firmware layers, later-milestone folders).
- CAN contract `body.dbc` v0.1 agreed (ADR 0002). Spec Kit inputs for constitution, spec 001 and spec 002 drafted.
- Spec Kit initialized.
- **Next:** review constitution input → `/speckit-constitution`; generate the CubeMX project for the STM32F407G-DISC1 and flash a plain blink.
