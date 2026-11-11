# ADR 0008: Extended bench topology (rear zone, LIN node, instrument cluster, door module)

## Status

Accepted (2026-11-11, maintainer decision). Extends [ADR 0003](0003-bench-network-topology.md); everything in ADR 0003 stays valid. Link media and function allocations marked *open* below are decided by later ADRs.

## Context

ADR 0003 defines a central HPC star with one CAN body zone (ZC-STM32), an Ethernet sensor zone (ZC-Arduino), an ADAS domain and a cloud tier. At electronica 2026 the bench gained automotive-grade and HMI hardware ([BOM](../hardware/bom.md)): an NXP FRDM-A-S32K344, an NXP FRDM-MCXA156, a Riverdi 5″ STM32U5 display and two Renesas FPB-RA2E3 kits with PTX105R NFC reader and servo.

The maintainer wants the target E/E architecture visible in the main topology from the start, so that no part of the goal is lost while the bench is built step by step. Production SDV architectures also have more than one zone, sub-buses below a zone (LIN), an instrument cluster and access control. These show the "zone + sub-bus + central compute" pattern that one body zone alone cannot.

## Decision

The bench topology is extended with four nodes:

| Node | Hardware | Role | Link | Status of the link |
|---|---|---|---|---|
| **ZC-S32K** (ZC02, rear body zone) | NXP FRDM-A-S32K344 | Second body zone controller: rear lighting, LIN master, UDS server | **CAN FD** to the HPC, on the second channel of the HPC's CAN interface (own bus, separate from the classic body CAN) | Decided |
| **LIN-MCXA** (LIN smart node) | NXP FRDM-MCXA156 | Smart sensor/actuator below the rear zone (e.g. mirror, ambient light) | **LIN**, ZC-S32K is master | Decided |
| **Cluster** (instrument cluster) | Riverdi 5″ STM32 Embedded Display (STM32U5), LVGL | Driver information: tell-tales, vehicle state, update progress, active DTCs | CAN FD or Ethernet | Open |
| **ZC-Door** (door / digital-key module) | Renesas FPB-RA2E3 + PTX105R NFC reader + SG90 servo | Reads the NFC key, drives the door lock, reports key events to the CGW | Classic **body CAN** through an MCP2515 SPI-CAN module | Open (alternative: UART to ZC-S32K) |

1. The bench stays a **central HPC star**. New: a zone may own a **sub-bus** (LIN below ZC-S32K), and the HPC drives **two CAN buses**: the classic body CAN (ZC-STM32, ZC-Door) and a CAN FD bus (ZC-S32K, possibly the cluster).
2. The **CGW on the HPC stays the single master of the vehicle state** (ADR 0006). The door module and the cluster are inputs or displays for the CGW, never a second mode source.
3. ZC-STM32 keeps all decisions of ADR 0005 and ADR 0006. **No function is moved** by this ADR.
4. The second Renesas kit is the development and spare unit of ZC-Door. It is not a separate node.

## Consequences

- The main network view (`docs/architecture/network/`), the deployment view, the README and the website topology show the extended topology. The separate "target topology" picture is removed.
- New requirement areas and nodes are introduced (see `docs/requirements/README.md`): areas `ACC` (access) and `HMI` (driver information); nodes `ZC-S32K`, `LIN-MCXA`, `Cluster`, `ZC-Door`. New stakeholder needs and system requirements are added as `Draft` without a milestone. Function and component requirements follow when a node's milestone starts.
- Hardware to add to the BOM: a LIN transceiver for each LIN node (the FRDM boards may not have one; check the board manuals), and a CAN interface for the HPC with two channels, one CAN FD capable (already the suggested Waveshare 2-CH CAN FD HAT).
- The MCP2515 supports **classic CAN only**. It can therefore join the body CAN, but not the CAN FD bus.
- New contracts are needed before any code: the rear-zone CAN FD frames (new DBC), door key events and lock command (body DBC version bump), the LIN schedule (LDF) and the cluster's input signals. Each comes with its own ADR (contracts-first rule).
- Milestones for the new nodes are not planned yet. Until they are, the current focus stays M1 (ZC-STM32).

## Open points

- **Rear indication across two zones.** ADR 0005 keeps turn indication fully inside ZC-STM32. Rear lamps on ZC-S32K need a blink phase in step with the front: does ZC-STM32 send its blink phase on the bus, or does each zone follow a central request? Decide with the first ZC-S32K feature; it may revise ADR 0005.
- **Cluster link and role.** CAN FD or Ethernet; display only, or also the state-setting dashboard of ADR 0006 (then it must remain an input to the CGW).
- **Door link.** Body CAN via MCP2515 or UART to ZC-S32K. Also check whether the RA2E3 has an on-chip CAN controller.
- **Key security.** Matching only the card's UID is a demo limitation. A real digital key needs authenticated cards (possible later topic, `RELEVANCE: Security`).
- **LIN-MCXA function.** Mirror, ambient light, or rest-bus / fault-injection node for testing the fail-safe.
