# Quickstart: verify spec 001 on the board

Everything here uses only the STM32F407G-DISC1 itself and its USB cable.

## 1. Build and flash

From `ecus/body-zone-stm32/` in the VS Code terminal:

```powershell
cube-cmake --preset Debug                 # once, or after pulling new files
cube-cmake --build --preset Debug
cube programmer -c port=SWD -w build/Debug/body-zone-stm32.elf -v -rst
```

Because new source files were added, run the configure step again even if you configured before.

## 2. Observe (SC-001, SC-002)

| Do | Expect |
|---|---|
| Press the black **reset** button | All four LEDs on for about 1 s, then all off |
| Wait | Blue LED blinks steadily: 0.5 s on, 0.5 s off |
| Press the blue **user** button 10 times, slow and fast | Orange LED changes state on each press, 10 times; off again at the end |
| Hold the blue button, press reset, keep holding through the lamp test, then let go | No orange change while holding; the next press toggles normally |
| Tap the blue button very briefly (< 50 ms) | Usually no toggle — that's the debounce filtering it out |

## 3. Debugger counters (SC-003)

1. Start a debug session (Run and Debug → the STM32 launch config), then Continue.
2. In **Watch** / **Live expressions**, add `g_sched_uptime_ms`, `g_sched_task100_count`, `g_ecu_state`.
3. Start a stopwatch together with the program. After 60 s: `g_sched_uptime_ms` ≈ 60000 (59 400–60 600 is a pass), `g_sched_task100_count` ≈ 600, `g_ecu_state` = `ECU_STATE_NORMAL`.

## 4. Host tests (optional, any PC with CMake and a C compiler)

```bash
cd ecus/body-zone-stm32/tests
cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure
```

## If something looks wrong

- **Nothing lights at all**: the flash step probably failed; re-run it and check the "Download verified successfully" line.
- **LEDs stay on**: the program is stuck before the main loop — start the debugger and pause to see where.
- **Orange toggles twice per press**: tell me; the debounce threshold would need checking against your button.
