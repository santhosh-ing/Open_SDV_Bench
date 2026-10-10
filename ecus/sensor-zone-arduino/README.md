# Sensor zone controller – Arduino (milestone M5)

Arduino + Ethernet shield. Reads a potentiometer ("vehicle speed") and a distance sensor and publishes them to the central HPC over Ethernet (MQTT), according to a contract in `interfaces/`. Built with PlatformIO. Topology: [ADR 0003](../../docs/adr/0003-bench-network-topology.md).
