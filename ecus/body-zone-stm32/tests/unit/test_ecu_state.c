/**
 * @file    test_ecu_state.c
 * @brief   Host tests for start-up lamp test, heartbeat and the bring-up button demo.
 *
 * Spec 001-body-ecu-bringup: FR-001, FR-002, FR-005, FR-007.
 * @verifies SW-ZCS-022 (Normal state, 1 Hz; the 4 Hz Degraded part follows with spec 002)
 */
#include "ecu_state.h"
#include "fake_bsp_led.h"
#include "unity.h"

#define CYCLE_MS ECU_STATE_TASK_PERIOD_MS

void setUp(void)
{
    fake_bsp_led_reset();
    ecu_state_init();
}
void tearDown(void) {}

static void run_ms(uint32_t ms, bool press_each_cycle)
{
    for (uint32_t i = 0U; i < ms / CYCLE_MS; i++)
    {
        ecu_state_task_10ms(press_each_cycle);
    }
}

static bool all_leds(bool on)
{
    return (fake_bsp_led_is_on(BSP_LED_GREEN) == on) && (fake_bsp_led_is_on(BSP_LED_ORANGE) == on) &&
           (fake_bsp_led_is_on(BSP_LED_RED) == on) && (fake_bsp_led_is_on(BSP_LED_BLUE) == on);
}

/* FR-001, FR-007: lamp test keeps every LED on for 1000 ms in state Init. */
void test_lamp_test_all_on_for_1000ms(void)
{
    TEST_ASSERT_EQUAL(ECU_STATE_INIT, ecu_state_get());
    TEST_ASSERT_TRUE(all_leds(true));
    run_ms(990U, false);
    TEST_ASSERT_TRUE(all_leds(true));
    TEST_ASSERT_EQUAL(ECU_STATE_INIT, ecu_state_get());
}

/* FR-001, FR-002, FR-007: after 1000 ms the lamp test ends and Normal starts. */
void test_lamp_test_ends_and_normal_starts(void)
{
    run_ms(1000U, false);
    TEST_ASSERT_EQUAL(ECU_STATE_NORMAL, ecu_state_get());
    TEST_ASSERT_FALSE(fake_bsp_led_is_on(BSP_LED_GREEN));
    TEST_ASSERT_FALSE(fake_bsp_led_is_on(BSP_LED_ORANGE));
    TEST_ASSERT_FALSE(fake_bsp_led_is_on(BSP_LED_RED));
}

/* @verifies SW-ZCS-022 — FR-002: blue heartbeat 500 ms on / 500 ms off. */
void test_heartbeat_1hz_50_percent(void)
{
    run_ms(1000U, false);
    uint32_t on_cycles = 0U;
    uint32_t edges = 0U;
    bool last = fake_bsp_led_is_on(BSP_LED_BLUE);
    for (uint32_t i = 0U; i < 1000U; i++) /* 10 s */
    {
        on_cycles += fake_bsp_led_is_on(BSP_LED_BLUE) ? 1U : 0U;
        ecu_state_task_10ms(false);
        bool now = fake_bsp_led_is_on(BSP_LED_BLUE);
        if (now && !last)
        {
            edges++;
        }
        last = now;
    }
    TEST_ASSERT_EQUAL_UINT32(500U, on_cycles);  /* 50 % duty */
    TEST_ASSERT_EQUAL_UINT32(10U, edges);       /* 10 rising edges in 10 s = 1 Hz */
}

/* FR-005: each press event toggles the orange LED in Normal. */
void test_button_event_toggles_orange(void)
{
    run_ms(1000U, false);
    for (uint32_t p = 0U; p < 10U; p++)
    {
        ecu_state_task_10ms(true);
        run_ms(200U, false);
    }
    TEST_ASSERT_EQUAL_UINT32(10U, fake_bsp_led_toggle_count(BSP_LED_ORANGE));
    TEST_ASSERT_FALSE(fake_bsp_led_is_on(BSP_LED_ORANGE)); /* even number of toggles */
}

/* Spec edge case: presses during the lamp test are ignored. */
void test_button_ignored_during_lamp_test(void)
{
    run_ms(500U, true);
    TEST_ASSERT_EQUAL_UINT32(0U, fake_bsp_led_toggle_count(BSP_LED_ORANGE));
    run_ms(500U, false);
    TEST_ASSERT_FALSE(fake_bsp_led_is_on(BSP_LED_ORANGE));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_lamp_test_all_on_for_1000ms);
    RUN_TEST(test_lamp_test_ends_and_normal_starts);
    RUN_TEST(test_heartbeat_1hz_50_percent);
    RUN_TEST(test_button_event_toggles_orange);
    RUN_TEST(test_button_ignored_during_lamp_test);
    return UNITY_END();
}
