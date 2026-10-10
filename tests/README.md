# System and integration tests

End-to-end tests across ECUs and the HPC (e.g. VSS request → CAN → lamp state), run against virtual twins in CI and against real hardware on the bench. Each test names the requirement it verifies with a `@verifies <ID>` marker (see `docs/requirements/README.md`).
