# Host unit tests

Unit tests for `app/functions/` and `app/com/` that run on a PC without hardware (CMake + CTest + Unity). Hardware access is replaced by fakes behind the `bsp`/`com` interfaces. Each test names the requirement or spec FR it verifies.

## Run

```bash
cmake -S . -B build && cmake --build build && ctest --test-dir build --output-on-failure
```

Unity is downloaded by CMake on the first configure. `unit/` holds one test file per module, `fakes/` the host stand-ins for `bsp` interfaces. The `firmware` CI workflow runs these on every pull request.
