# Interfaces (contracts)

The contracts between all parts of the bench. They are written **before** the code that implements them, and change only together with an ADR and a version bump.

| Folder | Contract | Status |
|---|---|---|
| `can/body.dbc` | CAN matrix of the body zone (v0.1.0) | Agreed (ADR 0002) |
| `diag/body_ecu.yaml` | Diagnostic description of the body ECU (v0.1.0) | Draft, detailed in M3 |
| `vss/` | VSS overlay for bench-specific signals | Planned (M2) |

Validate: `python3 -m cantools dump interfaces/can/body.dbc` (also runs in CI).
