#include "unity.h"

#include <stdint.h>

#include "mode_filter.h"

void test_mode_filter__find_mode_of_sensor_values_converted_to_cm_with_one_mode(void) {
  uint16_t filtered_sensor_value = 0;
  uint16_t *array_ptr;

  uint16_t input_buffer[12] = {69, 70, 71, 72, 70, 43, 72, 70, 68, 70, 69, 52};
  array_ptr = input_buffer;
  int buffer_size = sizeof(input_buffer) / sizeof(input_buffer[0]);

  filtered_sensor_value = mode_filter__find_mode_of_sensor_values_converted_to_cm(array_ptr, buffer_size);

  TEST_ASSERT_EQUAL_UINT16(70, filtered_sensor_value);
}

void test_mode_filter__find_mode_of_sensor_values_converted_to_cm_with_two_modes(void) {
  uint16_t filtered_sensor_value = 0;
  uint16_t *array_ptr;

  uint16_t input_buffer[15] = {71, 69, 70, 71, 72, 70, 43, 72, 70, 68, 70, 71, 69, 71, 52};
  array_ptr = input_buffer;
  int buffer_size = sizeof(input_buffer) / sizeof(input_buffer[0]);

  filtered_sensor_value = mode_filter__find_mode_of_sensor_values_converted_to_cm(array_ptr, buffer_size);

  TEST_ASSERT_EQUAL_UINT16(71, filtered_sensor_value);
}

void test_mode_filter__find_mode_of_sensor_values_converted_to_cm_with_five_modes(void) {
  uint16_t filtered_sensor_value = 0;
  uint16_t *array_ptr;

  uint16_t input_buffer[24] = {68, 71, 69, 70, 71, 72, 70,  43, 72, 70, 68, 52,
                               71, 69, 71, 70, 72, 72, 103, 69, 68, 68, 69, 12};
  array_ptr = input_buffer;
  int buffer_size = sizeof(input_buffer) / sizeof(input_buffer[0]);

  filtered_sensor_value = mode_filter__find_mode_of_sensor_values_converted_to_cm(array_ptr, buffer_size);

  TEST_ASSERT_EQUAL_UINT16(69, filtered_sensor_value);
}

void test_mode_filter__find_mode_of_sensor_values_converted_to_cm_with_same_values(void) {
  uint16_t filtered_sensor_value = 0;
  uint16_t *array_ptr;

  uint16_t input_buffer[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  array_ptr = input_buffer;
  int buffer_size = sizeof(input_buffer) / sizeof(input_buffer[0]);

  filtered_sensor_value = mode_filter__find_mode_of_sensor_values_converted_to_cm(array_ptr, buffer_size);

  TEST_ASSERT_EQUAL_UINT16(0, filtered_sensor_value);
}

void test_mode_filter__find_mode_of_sensor_values_converted_to_cm_with_no_repeat_values(void) {
  uint16_t filtered_sensor_value = 0;
  uint16_t *array_ptr;

  uint16_t input_buffer[20] = {0, 89, 90, 70, 68, 43, 91, 88, 87, 76, 74, 85, 84, 83, 99, 100, 73, 50, 66, 78};
  array_ptr = input_buffer;
  int buffer_size = sizeof(input_buffer) / sizeof(input_buffer[0]);

  filtered_sensor_value = mode_filter__find_mode_of_sensor_values_converted_to_cm(array_ptr, buffer_size);

  TEST_ASSERT_EQUAL_UINT16(78, filtered_sensor_value);
}

void test_mode_filter__find_mode_of_sensor_values_converted_to_cm_with_one_repeat_value(void) {
  uint16_t filtered_sensor_value = 0;
  uint16_t *array_ptr;

  uint16_t input_buffer[20] = {46, 46, 90, 70, 68, 43, 91, 88, 87, 76, 74, 85, 84, 83, 99, 100, 73, 50, 66, 78};
  array_ptr = input_buffer;
  int buffer_size = sizeof(input_buffer) / sizeof(input_buffer[0]);

  filtered_sensor_value = mode_filter__find_mode_of_sensor_values_converted_to_cm(array_ptr, buffer_size);

  TEST_ASSERT_EQUAL_UINT16(46, filtered_sensor_value);
}