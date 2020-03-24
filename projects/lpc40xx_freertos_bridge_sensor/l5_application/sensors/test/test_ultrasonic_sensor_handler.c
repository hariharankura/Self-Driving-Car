#include "unity.h"

#include "Mockadc.h"
#include "Mockdelay.h"
#include "Mockgpio.h"
#include <stdint.h>

#include "ultrasonic_sensor_handler.h"

void test_ultrasonic_sensor_handler__initialize_sensors(void) {
  gpio_s gpio;
  gpio__construct_with_function_ExpectAnyArgsAndReturn(gpio);
  gpio__construct_with_function_ExpectAnyArgsAndReturn(gpio);
  gpio__construct_with_function_ExpectAnyArgsAndReturn(gpio);
  gpio__construct_with_function_ExpectAnyArgsAndReturn(gpio);
  adc__initialize_Expect();
  ultrasonic_sensor_handler__initialize_sensors();
}

void test_ultrasonic_sensor_handler__convert_12_bit_adc_value_to_cm(void) {
  TEST_ASSERT_EQUAL_UINT16(-9, ultrasonic_sensor_handler__convert_12_bit_adc_value_to_cm(0));
  TEST_ASSERT_EQUAL_UINT16(308, ultrasonic_sensor_handler__convert_12_bit_adc_value_to_cm(4095));
  TEST_ASSERT_EQUAL_UINT16(13, ultrasonic_sensor_handler__convert_12_bit_adc_value_to_cm(300));
  TEST_ASSERT_EQUAL_UINT16(190, ultrasonic_sensor_handler__convert_12_bit_adc_value_to_cm(2574));
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