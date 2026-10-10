# Open SDV Bench

An open-source **software-defined vehicle on your desk**: real zone ECUs on a CAN bus, a central vehicle computer running Eclipse SDV components, and service-oriented diagnostics, built with the discipline of automotive systems engineering (requirements → architecture → decisions → code → test).

> Status: **M0 – Foundation** (architecture and contracts before code)

## Architecture

```
┌──────────── Off-board ─────────────┐
│ Diagnostic tester (SOVD REST)      │
└─────────────────┬──────────────────┘
┌──────── HPC (Raspberry Pi 5) ──────────────────────────┐
│ Eclipse OpenSOVD gateway ── classic adapter (SOVD→UDS) │
│ Eclipse Kuksa Databroker (VSS) ⇄ CAN provider (DBC)    │
│ Vehicle functions (apps)                               │
└─────────────────┬──────────────────────────────────────┘
                  │ CAN 500 kbit/s  ← interfaces/can/body.dbc
     ┌────────────┴─────────────┐
 Body zone ECU (STM32F407)   Sensor zone ECU (Arduino)
 lights, mirror servo, UDS   speed, distance
                  ▲
 ADAS domain (Jetson Orin) ── publishes to the Databroker
```

Every hardware ECU gets a **virtual twin** on virtual CAN, so the whole vehicle can run without hardware.

## Roadmap

| | Milestone | Outcome |
|---|---|---|
| M0 | Foundation | Requirements, architecture, contracts, repo and tooling |
| M1 | Body ECU on the bench | STM32 bring-up and light control over CAN (loopback) |
| M2 | Walking skeleton | VSS signal → Kuksa → CAN → ECU, end to end |
| M3 | Classic diagnostics | UDS server on the body ECU |
| M4 | Service-oriented diagnostics | Faults readable via Eclipse OpenSOVD REST |
| M5 | Vehicle function | Emergency brake → hazard lights, traced to requirements |
| M6 | ADAS domain | Jetson perception feeds the Databroker |
| M7 | Orchestration | Eclipse Ankaios manages vehicle functions |

## Repository layout

| Path | Contents |
|---|---|
| `docs/` | Requirements, architecture views, ADRs, hardware, Spec Kit inputs, devlog |
| `interfaces/` | Contracts: CAN (DBC), VSS, diagnostics |
| `specs/` | Spec Kit feature specifications |
| `ecus/` | Firmware: body zone (STM32), sensor zone (Arduino), virtual twins |
| `hpc/`, `adas-jetson/`, `deploy/` | Central computer, ADAS domain, deployment (later milestones) |
| `tests/` | System and integration tests |
| `tools/` | Helper scripts (e.g. CAN code generation) |

## Quickstart

```bash
# Inspect the CAN contract
pip install cantools
python3 -m cantools dump interfaces/can/body.dbc

# Generate the C pack/unpack code for the body ECU from the DBC
./tools/gen_can.sh
```

Or open the repo in VS Code and choose **Reopen in Container**; all tools come preinstalled.
Firmware flashing (STM32Cube for VS Code, ST-LINK over USB) runs on your host, not in the container.

## How this project is built

- **Contracts first**: CAN, signal and diagnostic interfaces are defined in `interfaces/` before any implementation.
- **Spec-driven development** with [GitHub Spec Kit](https://github.com/github/spec-kit) (pinned to v1.1.3): each feature goes from spec → plan → tasks → implementation, then verification on real hardware. Inputs for each spec are kept in `docs/spec-inputs/`.
- **Decisions are recorded** as ADRs in `docs/adr/`.
- Rules for AI coding assistants: [AGENTS.md](AGENTS.md).

## License

[MIT](LICENSE)
