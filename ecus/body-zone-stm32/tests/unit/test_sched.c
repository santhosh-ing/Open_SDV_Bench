/**
 * @file    test_sched.c
 * @brief   Host tests for the cooperative scheduler.
 *
 * Spec 001-body-ecu-bringup: FR-003 (1/10/100 ms slots, no drift), FR-006 (counters).
 * @verifies SW-ZCS-023
 */
#include "sched.h"
#include "unity.h"

static uint32_t runs_1ms, runs_10ms, runs_100ms;
static void t1(void) { runs_1ms++; }
static void t10(void) { runs_10ms++; }
static void t100(void) { runs_100ms++; }

static const sched_task_fn tasks_1[] = { t1 };
static const sched_task_fn tasks_10[] = { t10 };
static const sched_task_fn tasks_100[] = { t100 };
static const sched_config_t cfg = { tasks_1, 1U, tasks_10, 1U, tasks_100, 1U };

void setUp(void)
{
    runs_1ms = runs_10ms = runs_100ms = 0U;
}
void tearDown(void) {}

/* @verifies SW-ZCS-023 — FR-003: every slot runs at its period. */
void test_slots_run_at_their_periods(void)
{
    sched_init(&cfg, 0U);
    for (uint32_t t = 1U; t <= 1000U; t++)
    {
        sched_poll(t);
    }
    TEST_ASSERT_EQUAL_UINT32(1000U, runs_1ms);
    TEST_ASSERT_EQUAL_UINT32(100U, runs_10ms);
    TEST_ASSERT_EQUAL_UINT32(10U, runs_100ms);
}

/* @verifies SW-ZCS-023 — FR-003: a late main loop catches up, no drift accumulates. */
void test_late_polls_catch_up_without_drift(void)
{
    sched_init(&cfg, 0U);
    /* irregular polling: sometimes 7 ms late, sometimes on time */
    uint32_t t = 0U;
    while (t < 60000U)
    {
        t += ((t / 13U) % 3U == 0U) ? 7U : 1U;
        if (t > 60000U) { t = 60000U; }
        sched_poll(t);
    }
    TEST_ASSERT_EQUAL_UINT32(60000U, runs_1ms);
    TEST_ASSERT_EQUAL_UINT32(6000U, runs_10ms);
    TEST_ASSERT_EQUAL_UINT32(600U, runs_100ms);
}

/* FR-003: polling twice in the same millisecond runs nothing extra. */
void test_same_tick_runs_nothing(void)
{
    sched_init(&cfg, 5U);
    sched_poll(5U);
    sched_poll(5U);
    TEST_ASSERT_EQUAL_UINT32(0U, runs_1ms);
}

/* FR-006: debugger counters track uptime and 100 ms executions. */
void test_debug_counters(void)
{
    sched_init(&cfg, 0U);
    sched_poll(60000U);
    TEST_ASSERT_EQUAL_UINT32(60000U, g_sched_uptime_ms);
    TEST_ASSERT_EQUAL_UINT32(600U, g_sched_task100_count);
}

/* Spec edge case: the ms counter wraps after ~49.7 days. */
void test_counter_wrap(void)
{
    sched_init(&cfg, 0xFFFFFFF0U);
    sched_poll(0x00000010U); /* 32 ms later, across the wrap */
    TEST_ASSERT_EQUAL_UINT32(32U, runs_1ms);
    TEST_ASSERT_EQUAL_UINT32(3U, runs_10ms);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_slots_run_at_their_periods);
    RUN_TEST(test_late_polls_catch_up_without_drift);
    RUN_TEST(test_same_tick_runs_nothing);
    RUN_TEST(test_debug_counters);
    RUN_TEST(test_counter_wrap);
    return UNITY_END();
}
