#include "unity.h"

#include "Mockadc.h"
#include "ultrasonic_sensor_handler.h"
#include <stdint.h>

void test_ultrasonic_sensor_handler__initialize_sensors(void) { ultrasonic_sensor_handler__initialize_sensors(); }

void test_ultrasonic_sensor_handler__get_sensor_value_left(void) {
  uint16_t sensor_value = 0;

  ultrasonic_sensor_handler__get_sensor_value_left();
}

void test_ultrasonic_sensor_handler__get_sensor_value_right(void) {
  uint16_t sensor_value = 0;

  ultrasonic_sensor_handler__get_sensor_value_right();
}

void test_ultrasonic_sensor_handler__get_sensor_value_front(void) {
  uint16_t sensor_value = 0;

  ultrasonic_sensor_handler__get_sensor_value_front();
}

void test_ultrasonic_sensor_handler__get_sensor_value_back(void) {
  uint16_t sensor_value = 0;

  ultrasonic_sensor_handler__get_sensor_value_back();
}