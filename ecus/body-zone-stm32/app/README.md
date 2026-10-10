# Application code

Hand-written project code of the body ECU. Layers call **downward only**.

```
functions/   vehicle functions – pure logic, no HAL
    │
    ├── com/     CAN: driver wrapper, generated DBC code, timeouts, alive counters
    ├── diag/    UDS server (M3)
    └── bsp/     LEDs, button, servo – HAL access
sched/       1 ms cooperative scheduler, drives everything from app_run()
test_stim/   loopback-only test stimulus (BODY_CAN_LOOPBACK builds)
app.c/.h     app_init(), app_run() – the only symbols called from main.c
```

Modules are created by Spec Kit features (`specs/`), not up front. See `AGENTS.md` for the rules.
