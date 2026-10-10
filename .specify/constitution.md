# Open SDV Bench Constitution

## Mission

The mission of this repository is to enable traceable, layered ECU development for body-related vehicle features. Every feature must begin with a clear interface contract and design artifact before firmware implementation begins.

## Principles

1. Interface-first development: DBC, diagnostics, and feature contracts are created before implementation begins.
2. Generated code isolation: CubeMX-generated source must remain separate from handwritten application logic.
3. Layered architecture: board support, communication, diagnostics, and feature logic remain distinct.
4. Traceable delivery: each feature should have a spec, plan, and tasks artifact in `specs/`.
5. Minimum viable evidence: implementation changes should be documented and reviewable.

## Scope

This repository is intended for body ECU bring-up, CAN signaling, and later diagnostic service work. The final implementation should remain focused on automotive embedded patterns and predictable interfaces.
