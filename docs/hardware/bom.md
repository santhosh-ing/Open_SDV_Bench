# Bill of Materials – bench inventory

Status: **have** = on the bench · **ordered** · **needed** = to order.
Fill in exact models where marked `?` (M0 issue "Hardware inventory").

## Compute and ECUs

| Item | Exact model | Qty | Role | Status |
|---|---|---|---|---|
| STM32 board | STM32F407G-DISC1 (STM32F407VGT6, on-board ST-LINK) | 1 | Body zone ECU | have |
| Raspberry Pi | Raspberry Pi 5 (RAM: ?) | 1 | Central vehicle computer (HPC) | have |
| NVIDIA Jetson | Jetson Orin (variant: ?) | 1 | ADAS domain | have |
| Arduino | ? (model) | 2 | Sensor zone controller (ZC-Arduino, Ethernet) | have |

## Nodes added by ADR 0008 (electronica 2026)

Collected at electronica Munich on 2026-11-10 and allocated to the topology by [ADR 0008](../adr/0008-extended-bench-topology.md). No milestone yet; the current focus stays the ZC-STM32 (M1).

| Item | Exact model | Qty | Role (node) | Status |
|---|---|---|---|---|
| NXP FRDM Automotive board | FRDM-A-S32K344 (S32K344, Cortex-M7 lockstep, CAN FD, LIN, Ethernet, on-board debugger) | 1 | Rear body zone controller (ZC-S32K, ZC02), CAN FD, LIN master | have |
| NXP FRDM board | FRDM-MCXA156 (MCX A156, Cortex-M33 96 MHz, on-board MCU-Link) | 1 | LIN smart node (LIN-MCXA) below ZC-S32K | have |
| Riverdi STM32 Embedded Display | 5″ 800×480, STM32U599/U5A9 (exact Riverdi part no.: ?) | 1 | Instrument cluster (Cluster), LVGL | have |
| ST debug probe | ST-LINK (exact model: ?) | 1 | SWD debugger for the Riverdi display | have |
| Renesas Fast Prototyping Board | FPB-RA2E3 (RA2E3, Cortex-M23 48 MHz; confirm on silkscreen) | 2 | Door / digital-key module (ZC-Door); second kit = development and spare unit | have |
| NFC reader | Renesas PTX105R Pmod board, with NFC cards | 2 | NFC key reader of ZC-Door | have |
| Servo motor | SG90 | 2 | Door lock actuator of ZC-Door | have |

## Actuators, I/O and wiring

| Item | Exact model | Qty | Role | Status |
|---|---|---|---|---|
| LEDs | ? | ? | External lamps (later) | have |
| Servo motors | ? | ? | Mirror fold | have |
| Jumper wires | ? | – | Bench wiring | have |
| Joystick | KY-023-type 2-axis analogue module with push button | 1 | Turn-signal stalk (X axis) and hazard switch (push), ZC-STM32 (ADR 0005). Wiring: `wiring/joystick.yml` | have |

## To order (CAN bus and power)

| Item | Suggested model | Qty | Role | Status |
|---|---|---|---|---|
| CAN transceiver 3.3 V | SN65HVD230 module | 1–2 | STM32 to CAN bus | needed |
| CAN controller + transceiver | MCP2515 module | 2 | Arduino to CAN bus (not needed since ADR 0003; Arduino uses Ethernet). Planned body-CAN link of ZC-Door (ADR 0008; classic CAN only, not for the CAN FD bus) | have (8) |
| CAN interface for the Pi | 2-channel, at least one CAN FD capable (e.g. Waveshare 2-CH CAN FD HAT) | 1 | Pi to the body CAN and the CAN FD bus (SocketCAN), ADR 0008 | needed |
| Termination resistors | 120 Ω | 4 | Ends of the body CAN and the CAN FD bus | needed |
| LIN transceiver | e.g. TJA1021 / MCP2003 module (check the FRDM board manuals first) | 2 | ZC-S32K and LIN-MCXA to the LIN bus, ADR 0008 | needed |
| 12 V supply | – | 1 | LIN bus supply | needed |
| Twisted pair wire | – | – | CAN_H / CAN_L | needed |
| 5 V supply | ≥ 2 A | 1 | Servos (separate from logic boards) | needed |

## To order (Ethernet, ADR 0003)

| Item | Suggested model | Qty | Role | Status |
|---|---|---|---|---|
| Ethernet shield | W5500-based (depends on Arduino model) | 1 | ZC-Arduino to Ethernet | needed |
| Ethernet switch | 5-port unmanaged, 100 Mbit or faster | 1 | Pi, Jetson, Arduino (only if the cloud uplink does not use the Pi's Wi-Fi) | optional |
| Ethernet cables | Cat 5e patch cables | 3–4 | Bench Ethernet | needed |
