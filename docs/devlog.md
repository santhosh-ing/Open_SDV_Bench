# Devlog

Short, dated entries: what was done, what was learned, what's next. Newest at the top.

## 2026-10-10 (evening)

- Target architecture agreed: central HPC star with CAN to the STM32 body zone controller, Ethernet to the Jetson and the Arduino sensor zone, and a cloud tier (OpenSDV.Cloud) over IP (ADR 0003).
- Network/deployment view added as an editable draw.io diagram; README rewritten around the SDV principles the bench proves.
- **Next:** unchanged — `/speckit-constitution`, then CubeMX project and a plain blink on the STM32.

## 2026-10-10

- Repo aligned with the agreed structure (contracts, docs views, firmware layers, later-milestone folders).
- CAN contract `body.dbc` v0.1 agreed (ADR 0002). Spec Kit inputs for constitution, spec 001 and spec 002 drafted.
- Spec Kit initialized.
- **Next:** review constitution input → `/speckit-constitution`; generate the CubeMX project for the STM32F407G-DISC1 and flash a plain blink.
