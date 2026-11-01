# Wiring

Wiring harness diagrams as code with [WireViz](https://github.com/wireviz/WireViz): one YAML file per harness (e.g. `can-bus.yml`), rendered to a diagram and a bill of materials.

```bash
wireviz can-bus.yml
```

Harnesses:

| File | What | Status |
|---|---|---|
| `joystick.yml` | KY-023 joystick → STM32F407G-DISC1 (3V, GND, PA1, PA2, PE7) | on the bench |

Next harness: the CAN bus between STM32, Arduinos and the Raspberry Pi, once transceivers are on the bench.
