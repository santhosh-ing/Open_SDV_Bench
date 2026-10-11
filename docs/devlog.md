# Devlog

Short, dated entries: what was done, what was learned, what's next. Newest at the top.

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
