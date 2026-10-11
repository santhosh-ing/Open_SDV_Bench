# STM32 setup, build and flash

Hands-on steps to build the body zone firmware (`ecus/body-zone-stm32`) and flash it to the STM32F407G-DISC1. For what each step does and why, read [From code to blinking LED](firmware-workflow.md) first.

Tested on Windows 11 with VS Code and the STM32Cube for VS Code extensions. Commands work in PowerShell or Git Bash.

## One-time setup

1. **Install the extensions.** In VS Code, install **STM32Cube for Visual Studio Code**. It brings `cube-cmake` and the `cube` command line tool, and installs CMake, Ninja, the debugger and STM32CubeProgrammer as *bundles* under `%LOCALAPPDATA%\stm32cube\bundles`.

2. **Install the ARM compiler.** The compiler is a separate bundle and is not always installed with the extension:

   ```powershell
   cube bundle install gnu-tools-for-stm32
   ```

   Answer `yes` to accept ST's license. Check that it's there:

   ```powershell
   cube bundle list
   ```

   The list must include `gnu-tools-for-stm32` (14.3.1 at the time of writing).

3. **Connect the board.** Plug the Discovery board into USB using the **mini-USB connector (CN1)** next to the ST-LINK. Check that the PC sees it:

   ```powershell
   cube programmer -l st-link
   ```

   Expected: one probe listed with `Board Name : STM32F4DISCOVERY`.

## Configure

Run once, and again after adding source files or changing `CMakeLists.txt` or the presets. From `ecus/body-zone-stm32`:

```powershell
cube-cmake --preset Debug
```

Expected ending: `Build files have been written to: …/build/Debug`.

In VS Code: Command Palette → **CMake: Configure**.

## Everyday loop

From `ecus/body-zone-stm32`:

```powershell
# Build: compile and link → build/Debug/body-zone-stm32.elf
cube-cmake --build --preset Debug

# Flash: connect over SWD, write, verify, reset
cube programmer -c port=SWD -w build/Debug/body-zone-stm32.elf -v -rst
```

| Option | Meaning |
|---|---|
| `-c port=SWD` | Connect through the on-board ST-LINK using SWD |
| `-w <file>` | Write the program into flash |
| `-v` | Read the flash back and compare (verify) |
| `-rst` | Reset the chip so the new program starts |

A successful build ends with the linker's memory report (FLASH / RAM / CCMRAM usage). A successful flash prints `Download verified successfully` and `Software reset is performed`.

In VS Code: Command Palette → **CMake: Build**. Flashing and debugging from VS Code need a `.vscode/launch.json` with an STM32Cube ST-LINK configuration (Run and Debug → create a launch.json file → STM32Cube).

## Troubleshooting

| Symptom | Cause | Fix |
|---|---|---|
| `The CMAKE_C_COMPILER: arm-none-eabi-gcc is not a full path and was not found in the PATH` | The `gnu-tools-for-stm32` bundle is not installed | `cube bundle install gnu-tools-for-stm32`, then delete `build/Debug` and configure again |
| Configure keeps failing after a fix | `build/Debug/CMakeCache.txt` remembers the old failure | Delete `build/Debug` (or VS Code: **CMake: Delete Cache and Reconfigure**) |
| `ninja: no work to do` | Nothing changed since the last build | Not an error |
| `cube programmer -l st-link` lists no probe | USB cable, wrong connector, or driver | Use the mini-USB connector CN1, try another cable, reconnect; the ST-LINK LED (LD1) should light |
| Flash works but nothing happens on the board | Chip not restarted, or the program is stuck early in boot | Press the black RESET button (B2); attach a debugger to see where it stops |
| `No such command 'STM32_Programmer_CLI'` | The `cube` wrapper names it differently | Use `cube programmer …` |
