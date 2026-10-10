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

## Actuators, I/O and wiring

| Item | Exact model | Qty | Role | Status |
|---|---|---|---|---|
| LEDs | ? | ? | External lamps (later) | have |
| Servo motors | ? | ? | Mirror fold | have |
| Jumper wires | ? | – | Bench wiring | have |

## To order (CAN bus and power)

| Item | Suggested model | Qty | Role | Status |
|---|---|---|---|---|
| CAN transceiver 3.3 V | SN65HVD230 module | 1–2 | STM32 to CAN bus | needed |
| CAN controller + transceiver | MCP2515 module | 2 | Arduino to CAN bus (not needed since ADR 0003; Arduino uses Ethernet) | optional |
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
