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

## Candidate boards (electronica 2026, not yet allocated)

Collected at electronica Munich on 2026-11-10. These boards are **on the bench but have no role yet**. ADR 0003 (topology) is unchanged. Each candidate role below is an idea tracked as a `parking-lot` issue and becomes part of the topology only through its own ADR when its milestone starts.

| Item | Exact model | Qty | Candidate role (not decided) | Status |
|---|---|---|---|---|
| NXP FRDM Automotive board | FRDM-A-S32K344 (S32K344, Cortex-M7 lockstep, CAN FD, LIN, Ethernet, on-board debugger) | 1 | Second zone controller (rear zone) | have |
| NXP FRDM board | FRDM-MCXA156 (MCX A156, Cortex-M33 96 MHz, on-board MCU-Link) | 1 | LIN smart node or rest-bus / fault-injection node | have |
| Riverdi STM32 Embedded Display | 5″ 800×480, STM32U599/U5A9 (exact Riverdi part no.: ?) | 1 | Instrument cluster (tell-tales, vehicle state, DTCs), LVGL | have |
| ST debug probe | ST-LINK (exact model: ?) | 1 | SWD debugger for the Riverdi display | have |
| Renesas Fast Prototyping Board | FPB-RA2E3 (RA2E3, Cortex-M23 48 MHz; confirm on silkscreen) | 2 | Door / digital-key module | have |
| NFC reader | Renesas PTX105R Pmod board, with NFC cards | 2 | NFC key reader for the door module | have |
| Servo motor | SG90 | 2 | Door lock actuator | have |

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
| CAN controller + transceiver | MCP2515 module | 2 | Arduino to CAN bus (not needed since ADR 0003; Arduino uses Ethernet). Possible CAN link for the candidate Renesas door module (RA2E3 has no CAN, to be confirmed) | have (8) |
| CAN interface for the Pi | CAN HAT (e.g. Waveshare 2-CH CAN FD) or USB-CAN (candleLight) | 1 | Pi to CAN bus (SocketCAN) | needed |
| Termination resistors | 120 Ω | 2 | Bus ends | needed |
| Twisted pair wire | – | – | CAN_H / CAN_L | needed |
| 5 V supply | ≥ 2 A | 1 | Servos (separate from logic boards) | needed |

## To order (Ethernet, ADR 0003)

| Item | Suggested model | Qty | Role | Status |
|---|---|---|---|---|
| Ethernet shield | W5500-based (depends on Arduino model) | 1 | ZC-Arduino to Ethernet | needed |
| Ethernet switch | 5-port unmanaged, 100 Mbit or faster | 1 | Pi, Jetson, Arduino (only if the cloud uplink does not use the Pi's Wi-Fi) | optional |
| Ethernet cables | Cat 5e patch cables | 3–4 | Bench Ethernet | needed |
