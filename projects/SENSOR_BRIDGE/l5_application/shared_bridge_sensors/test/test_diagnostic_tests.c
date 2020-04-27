#include "unity.h"

#include "Mockled_handler.h"
#include "Mockultrasonic_sensor_handler.h"

#include "diagnostic_tests.h"

void test_diagnostic_tests__light_led3_when_obstacle_detected_10hz(void) {
  sensor_t sensor_values;
  ultrasonic_sensor_handler__get_all_sensor_values_ExpectAnyArgs();
  led_handler__diagnostic_test_object_detection_led3_Expect(sensor_values);
  diagnostic_tests__light_led3_when_obstacle_detected_10hz();
}

void test_diagnostic_tests__light_1_led_per_sensor_when_obstacle_detected_10hz(void) {
  sensor_t sensor_values;
  ultrasonic_sensor_handler__get_all_sensor_values_ExpectAnyArgs();
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor_Expect(sensor_values);
  diagnostic_tests__light_1_led_per_sensor_when_obstacle_detected_10hz();
}