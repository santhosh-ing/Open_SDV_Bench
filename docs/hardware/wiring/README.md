# Wiring

Wiring harness diagrams as code with [WireViz](https://github.com/wireviz/WireViz): one YAML file per harness (e.g. `can-bus.yml`), rendered to a diagram and a bill of materials.

```bash
wireviz can-bus.yml
```

First harness: the CAN bus between STM32, Arduinos and the Raspberry Pi, once transceivers are on the bench.
