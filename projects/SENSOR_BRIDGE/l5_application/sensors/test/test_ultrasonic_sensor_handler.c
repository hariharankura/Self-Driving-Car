#include "unity.h"

#include "Mockadc.h"
#include "Mockdelay.h"
#include "Mockgpio.h"
#include "Mockmode_filter.h"
#include <stdint.h>

#include "ultrasonic_sensor_handler.c"

void test_ultrasonic_sensor_handler__initialize_sensors(void) {
  gpio_s gpio;
  gpio__construct_with_function_ExpectAnyArgsAndReturn(gpio);
  gpio__construct_with_function_ExpectAnyArgsAndReturn(gpio);
  gpio__construct_with_function_ExpectAnyArgsAndReturn(gpio);
  gpio__construct_with_function_ExpectAnyArgsAndReturn(gpio);
  adc__initialize_Expect();
  ultrasonic_sensor_handler__initialize_sensors();
}

void test_ultrasonic_sensor_handler__3_3V_convert_12_bit_adc_value_to_cm(void) {
  TEST_ASSERT_EQUAL_UINT16(-21, ultrasonic_sensor_handler__3_3V_convert_12_bit_adc_value_to_cm(0));
  TEST_ASSERT_EQUAL_UINT16(495, ultrasonic_sensor_handler__3_3V_convert_12_bit_adc_value_to_cm(4095));
  TEST_ASSERT_EQUAL_UINT16(16, ultrasonic_sensor_handler__3_3V_convert_12_bit_adc_value_to_cm(300));
  TEST_ASSERT_EQUAL_UINT16(303, ultrasonic_sensor_handler__3_3V_convert_12_bit_adc_value_to_cm(2574));
}

void test_ultrasonic_sensor_handler__5V_convert_12_bit_adc_value_to_cm(void) {
  TEST_ASSERT_EQUAL_UINT16(-9, ultrasonic_sensor_handler__5V_convert_12_bit_adc_value_to_cm(0));
  TEST_ASSERT_EQUAL_UINT16(308, ultrasonic_sensor_handler__5V_convert_12_bit_adc_value_to_cm(4095));
  TEST_ASSERT_EQUAL_UINT16(13, ultrasonic_sensor_handler__5V_convert_12_bit_adc_value_to_cm(300));
  TEST_ASSERT_EQUAL_UINT16(190, ultrasonic_sensor_handler__5V_convert_12_bit_adc_value_to_cm(2574));
}

void test_ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left_first_value_above_threshold() {
  int threshold = 50;
  int numb_of_consec_values = 3;

  // set global variables back to initialized values when car is powered on
  first_sensor_reading_left = true;
  consec_values_below_threshold_left = 0;
  most_recent_sensor_value_above_threshold_left = 0;

  TEST_ASSERT_EQUAL_UINT16(
      70, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(70, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      80, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(80, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(
                                    100, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      30, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      47, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(47, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(90, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      37, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(37, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(50, threshold, numb_of_consec_values));
}

void test_ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left_first_value_below_threshold() {
  int threshold = 50;
  int numb_of_consec_values = 3;

  // set global variables back to initialized values when car is powered on
  first_sensor_reading_left = true;
  consec_values_below_threshold_left = 0;
  most_recent_sensor_value_above_threshold_left = 0;

  TEST_ASSERT_EQUAL_UINT16(threshold + 1, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(
                                              40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(threshold + 1, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(
                                              30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      40, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      80, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(80, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(
                                    100, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      30, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      47, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(47, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(90, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      37, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(37, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_left(90, threshold, numb_of_consec_values));
}

void test_ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right_first_value_above_threshold() {
  int threshold = 50;
  int numb_of_consec_values = 3;

  // set global variables back to initialized values when car is powered on
  first_sensor_reading_right = true;
  consec_values_below_threshold_right = 0;
  most_recent_sensor_value_above_threshold_right = 0;

  TEST_ASSERT_EQUAL_UINT16(
      70, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(70, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      80, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(80, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(
                                    100, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(
                                    50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(
                                    50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      30, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      47, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(47, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(90, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      37, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(37, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(50, threshold, numb_of_consec_values));
}

void test_ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right_first_value_below_threshold() {
  int threshold = 50;
  int numb_of_consec_values = 3;

  // set global variables back to initialized values when car is powered on
  first_sensor_reading_right = true;
  consec_values_below_threshold_right = 0;
  most_recent_sensor_value_above_threshold_right = 0;

  TEST_ASSERT_EQUAL_UINT16(threshold + 1, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(
                                              40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(threshold + 1, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(
                                              30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      40, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      80, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(80, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(
                                    100, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(
                                    50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(
                                    50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      30, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      47, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(47, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(90, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      37, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(37, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_right(90, threshold, numb_of_consec_values));
}

void test_ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front_first_value_above_threshold() {
  int threshold = 50;
  int numb_of_consec_values = 3;

  // set global variables back to initialized values when car is powered on
  first_sensor_reading_front = true;
  consec_values_below_threshold_front = 0;
  most_recent_sensor_value_above_threshold_front = 0;

  TEST_ASSERT_EQUAL_UINT16(
      70, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(70, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      80, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(80, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(
                                    100, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(
                                    50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(
                                    50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      30, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      47, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(47, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(90, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      37, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(37, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(50, threshold, numb_of_consec_values));
}

void test_ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front_first_value_below_threshold() {
  int threshold = 50;
  int numb_of_consec_values = 3;

  // set global variables back to initialized values when car is powered on
  first_sensor_reading_front = true;
  consec_values_below_threshold_front = 0;
  most_recent_sensor_value_above_threshold_front = 0;

  TEST_ASSERT_EQUAL_UINT16(threshold + 1, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(
                                              40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(threshold + 1, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(
                                              30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      40, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      80, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(80, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(
                                    100, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(
                                    50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(
                                    50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      30, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      47, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(47, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(90, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      37, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(37, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_front(90, threshold, numb_of_consec_values));
}

void test_ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back_first_value_above_threshold() {
  int threshold = 50;
  int numb_of_consec_values = 3;

  // set global variables back to initialized values when car is powered on
  first_sensor_reading_back = true;
  consec_values_below_threshold_back = 0;
  most_recent_sensor_value_above_threshold_back = 0;

  TEST_ASSERT_EQUAL_UINT16(
      70, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(70, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      80, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(80, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(
                                    100, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      30, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      47, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(47, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(90, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      37, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(37, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(50, threshold, numb_of_consec_values));
}

void test_ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back_first_value_below_threshold() {
  int threshold = 50;
  int numb_of_consec_values = 3;

  // set global variables back to initialized values when car is powered on
  first_sensor_reading_back = true;
  consec_values_below_threshold_back = 0;
  most_recent_sensor_value_above_threshold_back = 0;

  TEST_ASSERT_EQUAL_UINT16(threshold + 1, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(
                                              40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(threshold + 1, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(
                                              30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      40, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      80, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(80, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(
                                    100, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      100, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      30, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      47, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(47, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(90, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(40, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(30, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      37, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(37, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      50, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(50, threshold, numb_of_consec_values));
  TEST_ASSERT_EQUAL_UINT16(
      90, ultrasonic_sensor_handler__get_sensor_value_when_below_threshold_back(90, threshold, numb_of_consec_values));
}

void test_ultrasonic_sensor_handler__get_sensor_value_left(void) {
  uint16_t sensor_value = 0;

  adc__get_adc_value_ExpectAnyArgsAndReturn(sensor_value);

  ultrasonic_sensor_handler__get_sensor_value_left();
}

void test_ultrasonic_sensor_handler__get_sensor_value_right(void) {
  uint16_t sensor_value = 0;

  adc__get_adc_value_ExpectAnyArgsAndReturn(sensor_value);

  ultrasonic_sensor_handler__get_sensor_value_right();
}

void test_ultrasonic_sensor_handler__get_sensor_value_front(void) {
  uint16_t sensor_value = 0;

  adc__get_adc_value_ExpectAnyArgsAndReturn(sensor_value);

  ultrasonic_sensor_handler__get_sensor_value_front();
}

void test_ultrasonic_sensor_handler__get_sensor_value_back(void) {
  uint16_t sensor_value = 0;

  adc__get_adc_value_ExpectAnyArgsAndReturn(sensor_value);

  ultrasonic_sensor_handler__get_sensor_value_back();
}