#include "unity.h"

#include <stdio.h>

#include "Mockboard_io.h"
#include "Mockgpio.h"
#include "Mocklpc40xx.h"
#include "Mocklpc_peripherals.h"

#include "speed_sensor.c"

void setUp(void) {}

void tearDown(void) {}

void test_initialize_test_button_and_speed_sensor_interrupts(void) {
  gpio_s gpio_1;
  gpio__construct_as_input_ExpectAndReturn(0, 29, gpio_1);
  gpio__construct_as_input_ExpectAndReturn(0, 6, gpio_1);
  gpio_enable_interrupt_Expect(29);
  gpio_enable_interrupt_Expect(6);
  lpc_peripheral__enable_interrupt_Expect(LPC_PERIPHERAL__GPIO, handle_interrupts, NULL);
  initialize_test_button_and_speed_sensor_interrupts();
}

void test_motor_test_button_status(void) {
  motor_test_button_status = 1;
  TEST_ASSERT_EQUAL_UINT8(1, get_motor_test_button_status());

  reset_motor_test_button_status();
  TEST_ASSERT_EQUAL_UINT8(0, get_motor_test_button_status());
}

void test_clear_rotations_in_windowtime(void) {
  clear_rotations_in_windowtime();
  TEST_ASSERT_EQUAL(rotations_in_windowtime, 0);
}

void test_get_windowtime(void) { TEST_ASSERT_EQUAL(get_windowtime(), 1); }