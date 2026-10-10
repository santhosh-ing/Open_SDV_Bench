# sched – cooperative scheduler

1 ms time base from SysTick; task slots of 1 ms, 10 ms and 100 ms executed from `app_run()` in the main loop (never inside the interrupt). Replaces any blocking delay.
