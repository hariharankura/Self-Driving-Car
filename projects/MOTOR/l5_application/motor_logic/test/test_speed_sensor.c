#include "unity.h"

#include <stdio.h>

#include "Mockboard_io.h"
#include "Mockgpio.h"
#include "Mocklpc40xx.h"
#include "Mocklpc_peripherals.h"

#include "speed_sensor.c"

void setUp(void) {}

void tearDown(void) {}

void test_init_speed_sensor(void) {
  gpio_s gpio_1;

  gpio__construct_as_input_ExpectAndReturn(0, 29, gpio_1);
  gpio_enable_interrupt_Expect(29);
  lpc_peripheral__enable_interrupt_Expect(LPC_PERIPHERAL__GPIO, button_interrupt, NULL);
  board_io__get_led0_ExpectAndReturn(gpio_1);
  gpio__set_Expect(gpio_1);
  board_io__get_led1_ExpectAndReturn(gpio_1);
  gpio__set_Expect(gpio_1);
  board_io__get_led2_ExpectAndReturn(gpio_1);
  gpio__set_Expect(gpio_1);
  board_io__get_led3_ExpectAndReturn(gpio_1);
  gpio__set_Expect(gpio_1);
  init_speed_sensor();
}

void test_get_rpm(void) { TEST_ASSERT_EQUAL(get_rpm(), 0); }

void test_get_mph(void) { TEST_ASSERT_EQUAL(get_rpm(), 0); }

void test_clear_rotations_in_windowtime(void) {

  rpm = 1;
  recorded_rpm = 2;

  mph = 1;
  recorded_mph = 2;

  rotations_in_windowtime = 5;

  clear_rotations_in_windowtime();

  TEST_ASSERT_EQUAL(recorded_mph, mph);
  TEST_ASSERT_EQUAL(recorded_rpm, rpm);
  TEST_ASSERT_EQUAL(rotations_in_windowtime, 0);
}

void test_get_windowtime(void) { TEST_ASSERT_EQUAL(get_windowtime(), 5); }