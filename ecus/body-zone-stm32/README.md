# Body zone ECU – STM32F407G-DISC1

Lights (hazard, indicators, low beam), mirror servo (later) and UDS diagnostics (M3), implementing `interfaces/can/body.dbc`.

| Path | Contents | Edited by |
|---|---|---|
| `body-zone-stm32.ioc` | CubeMX hardware configuration | CubeMX only |
| `Core/`, `Drivers/` | CubeMX-generated code and ST HAL | CubeMX only (except `USER CODE` blocks in `main.c`) |
| `app/` | Project code, layered | You / Spec Kit |
| `tests/` | Host unit tests (no hardware) | You / Spec Kit |

## Create the CubeMX project (first time)

1. STM32CubeMX → **Board Selector** → `STM32F407G-DISC1` → initialize peripherals with default mode: **No** (we configure only what we need).
2. Clock: HSE 8 MHz → PLL → SYSCLK 168 MHz (APB1 42 MHz, APB2 84 MHz). Debug: Serial Wire.
3. GPIO: PD12–PD15 output (LEDs), PA0 input (button).
4. Project Manager: name `body-zone-stm32`, location = this folder's parent, toolchain **CMake**. Generate.
5. In `main.c`, inside `USER CODE` blocks only: call `app_init()` before the main loop and `app_run()` inside it.
6. Flash a plain blink first to prove the toolchain.

## Build and flash

From this folder, with the board connected over USB:

```powershell
cube-cmake --preset Debug                 # configure (first time, or after adding files)
cube-cmake --build --preset Debug         # build → build/Debug/body-zone-stm32.elf
cube programmer -c port=SWD -w build/Debug/body-zone-stm32.elf -v -rst   # flash, verify, reset
```

Tool setup and troubleshooting: [STM32 setup, build and flash](../../docs/guides/stm32-build-and-flash.md). What each step does: [From code to blinking LED](../../docs/guides/firmware-workflow.md).

## Regenerate CAN code from the contract

```bash
tools/gen_can.sh   # writes app/com/generated/
```
