#include "unity.h"

#include <stdio.h>

#include "Mockboard_io.h"
#include "Mockgpio.h"
#include "Mocklpc40xx.h"
#include "Mocklpc_peripherals.h"
#include "Mockmotor_self_test.h"

#include "speed_sensor.c"

void setUp(void) {}

void tearDown(void) {}

void test_initialize_speed_sensor_and_test_button_interrupts(void) {
  gpio_s gpio_1;
  gpio__construct_as_input_ExpectAndReturn(0, 6, gpio_1);
  gpio_enable_interrupt_Expect(6);
  gpio__construct_as_input_ExpectAndReturn(0, 29, gpio_1);
  gpio_enable_interrupt_Expect(29);
  lpc_peripheral__enable_interrupt_Expect(LPC_PERIPHERAL__GPIO, handle_interrupt, NULL);
  initialize_speed_sensor_and_test_button_interrupts();
}

void test_get_rpm(void) {
  rpm = 120;
  TEST_ASSERT_EQUAL_UINT32(120, get_rpm());

  rpm = 80;
  TEST_ASSERT_EQUAL_UINT32(80, get_rpm());
}

void test_get_mph(void) {
  mph = 3.2;
  TEST_ASSERT_EQUAL_FLOAT(3.2, get_mph());

  mph = 5.6;
  TEST_ASSERT_EQUAL_FLOAT(5.6, get_mph());
}

void test_calculate_rpm_and_mph(void) {
  calculate_rpm_and_mph(2);
  uint32_t test_rpm = 2 * ((60 * 1000) / windowtime_in_ms);
  float test_mph = ((pi * wheel_diameter_in_cm) * (test_rpm * 60)) / (miles_to_cm * gear_ratio);
  printf("mph:%f\n", test_mph);
  TEST_ASSERT_EQUAL_UINT32(test_rpm, get_rpm());
  TEST_ASSERT_EQUAL_FLOAT(test_mph, get_mph());

  calculate_rpm_and_mph(1);
  test_rpm = 1 * ((60 * 1000) / windowtime_in_ms);
  test_mph = ((pi * wheel_diameter_in_cm) * (test_rpm * 60)) / (miles_to_cm * gear_ratio);
  TEST_ASSERT_EQUAL_UINT32(test_rpm, get_rpm());
  TEST_ASSERT_EQUAL_FLOAT(test_mph, get_mph());
}

void test_clear_rotations_in_windowtime(void) {
  rotations_in_windowtime = 4;
  clear_rotations_in_windowtime();

  uint32_t test_rpm = 4 * ((60 * 1000) / windowtime_in_ms);
  float test_mph = ((pi * wheel_diameter_in_cm) * (test_rpm * 60)) / (miles_to_cm * gear_ratio);
  TEST_ASSERT_EQUAL_UINT32(test_rpm, get_rpm());
  TEST_ASSERT_EQUAL_FLOAT(test_mph, get_mph());

  TEST_ASSERT_EQUAL(0, rotations_in_windowtime);
}