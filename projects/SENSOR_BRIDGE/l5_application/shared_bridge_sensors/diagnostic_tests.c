#include "diagnostic_tests.h"
#include "led_handler.h"
#include "ultrasonic_sensor_handler.h"

void diagnostic_tests__light_led3_when_obstacle_detected_10hz(void) {
  sensor_t sensor_values;
  ultrasonic_sensor_handler__get_all_sensor_values(&sensor_values);
  led_handler__diagnostic_test_object_detection_led3(sensor_values);
}

void diagnostic_tests__light_1_led_per_sensor_when_obstacle_detected_10hz(void) {
  sensor_t sensor_values;
  ultrasonic_sensor_handler__get_all_sensor_values(&sensor_values);
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor(sensor_values);
}