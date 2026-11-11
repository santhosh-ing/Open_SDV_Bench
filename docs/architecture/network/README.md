# Network / topology view

Physical view of the bench: which nodes exist, how they are connected, and which protocol runs on each link. Decision records: [ADR 0003](../../adr/0003-bench-network-topology.md) (star topology) and [ADR 0008](../../adr/0008-extended-bench-topology.md) (rear zone, LIN node, instrument cluster, door module).

![Open SDV Bench network and deployment view](network-topology.drawio.svg)

The diagram is an editable draw.io file (`network-topology.drawio.svg`): open it in VS Code with the *Draw.io Integration* extension or at [app.diagrams.net](https://app.diagrams.net), edit, save. GitHub renders the same file as an image.

## Topology

A **central HPC star**: the Raspberry Pi 5 is the hub. Every zone controller and domain computer has one link to it, and the cloud connects through it. A zone may own a **sub-bus** below it (LIN below the rear zone). Each link uses the medium that fits its job:

- **Ethernet / IP** where services, containers or larger data are exchanged (cloud, ADAS, sensor zone).
- **CAN** where a small, deterministic, cyclic signal set and classic diagnostics are needed. The HPC drives two CAN buses: the classic **body CAN** (front body zone, door module) and a **CAN FD** bus (rear body zone).
- **LIN** for low-cost smart sensors and actuators below a zone controller.

## Nodes

| Node | Hardware | Role | Interfaces |
|---|---|---|---|
| OpenSDV.Cloud | Laptop or cloud VM | OTA update server, digital twin, remote diagnostics client | IP |
| Central HPC | Raspberry Pi 5 | Vehicle computer: VSS Databroker, vehicle apps, SOVD gateway, orchestration, cloud connector | Ethernet, Wi-Fi, CAN (via HAT) |
| ADAS domain | NVIDIA Jetson Orin | Camera perception, publishes detections as VSS signals | Ethernet |
| ZC-STM32 (ZC01) | STM32F407G-DISC1 | Front body zone controller: lighting, indicators, hazard, fail-safe, UDS server | CAN (bxCAN + 3.3 V transceiver) |
| ZC-Arduino | Arduino + Ethernet shield | Sensor zone controller: speed and distance sensing | Ethernet |
| ZC-S32K (ZC02) | NXP FRDM-A-S32K344 | Rear body zone controller: rear lighting, LIN master, UDS server | CAN FD, LIN |
| LIN-MCXA | NXP FRDM-MCXA156 | LIN smart node below the rear zone (mirror or ambient light, open) | LIN (+ transceiver) |
| Cluster | Riverdi 5″ STM32 Embedded Display (STM32U5) | Instrument cluster (LVGL): tell-tales, vehicle state, update progress, DTCs | CAN FD or Ethernet (open) |
| ZC-Door | Renesas FPB-RA2E3 + PTX105R NFC + SG90 servo | Door / digital-key module: NFC key, door lock, key events to the CGW | Body CAN via MCP2515 (open) |

## Links

| Link | Medium | Protocol (planned) | Carries | Milestone |
|---|---|---|---|---|
| Cloud ↔ HPC | IP (Wi-Fi uplink, like a vehicle's cellular link) | MQTT / HTTPS | Telemetry, OTA packages, remote SOVD | M8 (proposed) |
| HPC ↔ ZC-STM32 | CAN classic, 500 kbit/s, 120 Ω at both ends | Signals per `interfaces/can/body.dbc`; UDS over ISO-TP | Light commands and status, diagnostics | M1 (loopback), M2 (real bus), M3 (UDS) |
| HPC ↔ ADAS | Ethernet | gRPC (Kuksa Databroker API) | Detected objects as VSS signals | M6 |
| HPC ↔ ZC-Arduino | Ethernet | MQTT (lightweight, fits a small MCU) | Speed and distance signals | M5 |
| HPC ↔ ZC-S32K | CAN FD, second channel of the HPC CAN interface | New rear-zone DBC; UDS | Rear lighting, vehicle state, diagnostics | not planned yet |
| ZC-S32K ↔ LIN-MCXA | LIN, ZC-S32K is master | LIN schedule (LDF) | Smart node commands and status | not planned yet |
| HPC ↔ Cluster | CAN FD or Ethernet (open) | Cluster input signals (contract open) | Tell-tales, vehicle state, DTCs | not planned yet |
| HPC ↔ ZC-Door | Body CAN via MCP2515 (open; alternative UART to ZC-S32K) | `body.dbc` extension | Key events, lock command and status | not planned yet |

## Open points

- **Arduino model**: the exact model decides whether an Ethernet shield (e.g. W5500) is needed and how much protocol fits into its RAM. To be filled in `docs/hardware/bom.md`.
- **Ethernet ports**: the Pi 5 has one Ethernet port. With the Jetson and the Arduino on Ethernet, either a small unmanaged switch is added, or the cloud link uses the Pi's Wi-Fi (preferred, mirrors the vehicle's cellular uplink).
- **Protocols per link** are planned, not final. Each is confirmed by an ADR when its milestone starts.
- **ADR 0008 open points**: rear indication in step across two zones, cluster link and role, door link, key security, LIN node function.
- **MCP2515 is classic CAN only**: it fits the body CAN, not the CAN FD bus.

## Function-to-node mapping

See the [deployment view](../deployment/README.md).
