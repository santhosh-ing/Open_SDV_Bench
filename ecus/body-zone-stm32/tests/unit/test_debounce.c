/**
 * @file    test_debounce.c
 * @brief   Host tests for the button debounce logic.
 *
 * Spec 001-body-ecu-bringup: FR-004 (stable 50 ms, exactly one event per press).
 * Sample period 10 ms, threshold 5 samples = 50 ms.
 */
#include "debounce.h"
#include "unity.h"

#define THRESHOLD 5U

static debounce_t db;

void setUp(void) { debounce_init(&db, THRESHOLD, false); }
void tearDown(void) {}

static void feed(bool level, uint32_t samples)
{
    for (uint32_t i = 0U; i < samples; i++)
    {
        debounce_sample(&db, level);
    }
}

/* FR-004: press held for exactly 50 ms gives one event. */
void test_press_held_50ms_gives_one_event(void)
{
    feed(true, THRESHOLD - 1U);
    TEST_ASSERT_FALSE(debounce_take_press_event(&db));
    feed(true, 1U);
    TEST_ASSERT_TRUE(debounce_take_press_event(&db));
    TEST_ASSERT_FALSE(debounce_take_press_event(&db)); /* cleared on read */
}

/* FR-004: a press shorter than 50 ms is ignored. */
void test_short_press_is_ignored(void)
{
    feed(true, THRESHOLD - 1U);
    feed(false, 10U);
    TEST_ASSERT_FALSE(debounce_take_press_event(&db));
    TEST_ASSERT_FALSE(debounce_is_pressed(&db));
}

/* FR-004: contact bounce does not create extra events. */
void test_bouncing_contact_gives_one_event(void)
{
    const bool bounce[] = { true, false, true, true, false, true, false };
    for (uint32_t i = 0U; i < sizeof(bounce) / sizeof(bounce[0]); i++)
    {
        debounce_sample(&db, bounce[i]);
    }
    TEST_ASSERT_FALSE(debounce_take_press_event(&db));
    feed(true, THRESHOLD);
    TEST_ASSERT_TRUE(debounce_take_press_event(&db));
    /* bounce while held: still pressed, no new event */
    debounce_sample(&db, false);
    feed(true, 3U);
    TEST_ASSERT_FALSE(debounce_take_press_event(&db));
}

/* FR-004 / spec story 2 scenario 3: holding gives one event; next needs a release. */
void test_hold_then_release_then_press_gives_two_events(void)
{
    uint32_t events = 0U;
    for (uint32_t i = 0U; i < 300U; i++) /* held 3 s */
    {
        debounce_sample(&db, true);
        events += debounce_take_press_event(&db) ? 1U : 0U;
    }
    TEST_ASSERT_EQUAL_UINT32(1U, events);
    feed(false, THRESHOLD);
    TEST_ASSERT_FALSE(debounce_is_pressed(&db));
    feed(true, THRESHOLD);
    TEST_ASSERT_TRUE(debounce_take_press_event(&db));
}

/* Spec edge case: button held at reset gives no event until released and pressed again. */
void test_button_held_at_reset_gives_no_event(void)
{
    debounce_init(&db, THRESHOLD, true);
    feed(true, 50U);
    TEST_ASSERT_FALSE(debounce_take_press_event(&db));
    feed(false, THRESHOLD);
    feed(true, THRESHOLD);
    TEST_ASSERT_TRUE(debounce_take_press_event(&db));
}

/* SC-002 style: 10 clean presses -> exactly 10 events. */
void test_ten_presses_give_ten_events(void)
{
    uint32_t events = 0U;
    for (uint32_t p = 0U; p < 10U; p++)
    {
        for (uint32_t i = 0U; i < 12U; i++)
        {
            debounce_sample(&db, true);
            events += debounce_take_press_event(&db) ? 1U : 0U;
        }
        feed(false, 12U);
    }
    TEST_ASSERT_EQUAL_UINT32(10U, events);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_press_held_50ms_gives_one_event);
    RUN_TEST(test_short_press_is_ignored);
    RUN_TEST(test_bouncing_contact_gives_one_event);
    RUN_TEST(test_hold_then_release_then_press_gives_two_events);
    RUN_TEST(test_button_held_at_reset_gives_no_event);
    RUN_TEST(test_ten_presses_give_ten_events);
    return UNITY_END();
}
