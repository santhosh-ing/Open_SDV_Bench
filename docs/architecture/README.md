# Architecture (arc42)

Architecture documentation of Open SDV Bench, structured along arc42. Diagrams are kept as code next to the text.

## Views

Inspired by the layers of an E/E architecture tool (requirements → logical → software → network → deployment):

| View | Folder | Shows | Format |
|---|---|---|---|
| Logical (function) view | `logical/` | Vehicle functions and information flows, independent of hardware | PlantUML / Mermaid |
| Software view | `software/` | Software components and layers (C4 containers, firmware layers) | Mermaid / C4-PlantUML |
| Network / topology view | `network/` | ECUs, buses, connections, terminations | draw.io (`.drawio.svg`) |
| Deployment view | `deployment/` | Which function / workload runs on which ECU or computer | PlantUML / table |

## 1. Introduction and goals

A desktop software-defined-vehicle bench that demonstrates a realistic zone architecture: zone ECUs on CAN, a central vehicle computer with Eclipse SDV components (Kuksa Databroker + VSS, Eclipse OpenSOVD), and an ADAS domain. Quality goals: traceability, modularity, runnable without hardware (virtual twins).

## 2. Constraints

Hobby hardware (STM32F407G-DISC1, Raspberry Pi 5, Arduino, Jetson Orin), open-source software only, CAN classic at 500 kbit/s.

## 3. Context and scope

*To be completed (M0 issue "Diagram: system context").*

## 4. Solution strategy

Contracts first (`interfaces/`), spec-driven development (Spec Kit), layered firmware (ADR 0001), command/status CAN split (ADR 0002), virtual twin per ECU.

## 5. Building block view

Body zone ECU firmware layers: see [`software/body-ecu-layers.mmd`](software/body-ecu-layers.mmd) and [`../adr/0001-use-autosar-inspired-layering.md`](../adr/0001-use-autosar-inspired-layering.md).

## 6.–12. Runtime view, deployment, cross-cutting concepts, decisions, quality, risks, glossary

*Filled in as the milestones progress. Decisions: see [`../adr/`](../adr/).*
