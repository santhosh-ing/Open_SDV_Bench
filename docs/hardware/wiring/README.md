# Wiring

Wiring harness diagrams as code with [WireViz](https://github.com/wireviz/WireViz): one YAML file per harness (e.g. `can-bus.yml`), rendered to a diagram and a bill of materials.

```bash
wireviz can-bus.yml
```

Harnesses:

| File | What | Status |
|---|---|---|
| `joystick.yml` | KY-023 joystick → STM32F407G-DISC1 (3V, GND, PA1, PA2, PE7) | on the bench |

## Joystick connection (quick reference)

Wire with the board unpowered. Five female-female jumper wires.

| Wire | Joystick pin (KY-023) | STM32F407G-DISC1 pin | Purpose |
|---|---|---|---|
| black | GND | GND | Ground |
| red | +5V | **3V** (not 5V) | Supply. The module pin is labelled +5V, but it must get 3 V: the ADC accepts at most 3.3 V. |
| blue | VRy | PA1 | Left/right axis (ADC1_IN1), used for turn indication |
| violet | VRx | PA2 | Other axis (ADC1_IN2), reserved, not read yet |
| green | SW | PE7 | Push button = hazard switch (internal pull-up, pressed = low) |

The pin names are printed on the board next to headers P1 and P2. The wire colours are a suggestion; any colours work.

VRx and VRy are crossed on purpose: as the module is mounted, left/right moves the VRy output. This follows the first readings on the bench (2026-11-01) and is confirmed once `g_joy_raw_x` shows about 2048 at rest, below 1024 at one end and above 3072 at the other.

Next harness: the CAN bus between STM32, Arduinos and the Raspberry Pi, once transceivers are on the bench.
