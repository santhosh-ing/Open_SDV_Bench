# Host unit tests

Unit tests for `app/functions/` and `app/com/` that run on a PC without hardware (CMake + CTest + Unity). Hardware access is replaced by fakes behind the `bsp`/`com` interfaces. Each test names the requirement or spec FR it verifies.
