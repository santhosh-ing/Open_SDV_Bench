# ADR 0003: Bench network topology (central HPC star, CAN and Ethernet, cloud via IP)

## Status

Accepted

## Context

The first architecture sketch put every zone ECU on one CAN bus and only hinted at Ethernet. That under-represents what a software-defined vehicle looks like: production SDV architectures combine a central computer, zone controllers, Ethernet for services and high-bandwidth data, CAN for deterministic low-level signals, and a cloud backend for updates and remote access. The bench should show that mix, and diagnostics should be one capability proven on the setup rather than the whole story.

## Decision

The bench uses a **star topology around the central HPC** (Raspberry Pi 5):

| Link | Medium | Planned protocol |
|---|---|---|
| OpenSDV.Cloud ↔ HPC | IP (Wi-Fi uplink) | MQTT / HTTPS |
| HPC ↔ ADAS (Jetson Orin) | Ethernet | gRPC (Kuksa Databroker API) |
| HPC ↔ ZC-STM32 (body zone) | CAN classic, 500 kbit/s | `body.dbc` signals, UDS over ISO-TP |
| HPC ↔ ZC-Arduino (sensor zone) | Ethernet | MQTT |

- The STM32 stays on CAN: it needs no extra Ethernet PHY hardware, M1 is unchanged, and the body zone keeps a deterministic cyclic contract with timeout supervision (ADR 0002).
- The Arduino sensor zone moves from CAN to Ethernet. Its traffic stays lightweight (MQTT) to fit a small microcontroller.
- A cloud tier (**OpenSDV.Cloud**) is added for OTA updates, a digital twin and remote diagnostics (proposed milestone M8).
- Eclipse components per node (Kuksa, Velocitas, OpenSOVD, Ankaios, Kanto, hawkBit, Hono / Ditto) are **candidates**. Each one is confirmed by its own ADR when its milestone starts.

## Consequences

- New hardware: an Ethernet shield for the Arduino (model-dependent) and, unless the cloud link uses Wi-Fi, an Ethernet switch. The MCP2515 CAN modules are no longer required for the sensor zone. See `docs/hardware/bom.md`.
- The sensor-zone signal contract will live in `interfaces/` as an MQTT topic/payload description instead of a DBC.
- The architecture views (`docs/architecture/network/`, `deployment/`) and the README show this topology.
- Open: exact Arduino model; whether the Pi uses a switch or Wi-Fi for the cloud uplink.
