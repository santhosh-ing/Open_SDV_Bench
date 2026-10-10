# Deployment view

Which function or workload runs on which node. Physical connections: see the [network view](../network/README.md). Later, the HPC part of this table becomes Eclipse Ankaios workload manifests (milestone M7).

| Node | Function / workload | Technology (planned) | Milestone |
|---|---|---|---|
| ZC-STM32 | Low beam, left/right indicators, hazard | Firmware, `app/functions/` | M1 |
| ZC-STM32 | Local hazard input (user button), command-timeout fail-safe, ECU states | Firmware, `app/functions/`, `app/com/` | M1 |
| ZC-STM32 | UDS diagnostic server (DIDs, DTCs, IO control) | Firmware, `app/diag/` | M3 |
| ZC-STM32 | Mirror fold (servo) | Firmware, `app/functions/` | later |
| ZC-Arduino | Speed (potentiometer) and distance sensing | PlatformIO firmware, MQTT client | M5 |
| HPC | VSS Databroker | Eclipse Kuksa Databroker (container) | M2 |
| HPC | CAN ↔ VSS mapping | Kuksa CAN provider, `interfaces/can/body.dbc` | M2 |
| HPC | Vehicle apps: light control, brake assist | Python apps, Eclipse Velocitas model, Kuksa client | M2, M5 |
| HPC | SOVD gateway and classic adapter (SOVD → UDS) | Eclipse OpenSOVD | M4 |
| HPC | Workload orchestration and updates | Eclipse Ankaios | M7 |
| HPC | Cloud connector, OTA client | Eclipse Kanto | M8 (proposed) |
| ADAS | Camera perception, publishes detections as VSS | Container workload, Kuksa client | M6 |
| OpenSDV.Cloud | OTA update server | Eclipse hawkBit | M8 (proposed) |
| OpenSDV.Cloud | Device connectivity, digital twin | Eclipse Hono / Ditto | M8 (proposed) |
| OpenSDV.Cloud | Remote diagnostics | SOVD client against the HPC gateway | M8 (proposed) |
| Developer PC / CI | Virtual twins of the zone controllers | Python, `vcan0`, same contracts | M2 |

Components marked "planned" are candidates; each is confirmed by an ADR when its milestone starts.
