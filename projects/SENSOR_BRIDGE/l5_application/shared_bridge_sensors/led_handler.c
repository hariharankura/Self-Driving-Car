#include "led_handler.h"
#include "board_io.h"
#include "gpio.h"

void led_handler__turn_off_all_sjtwo_leds(void) {
  gpio__set(board_io__get_led0());
  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__set(board_io__get_led3());
}

void led_handler__diagnostic_test_object_detection_led3(sensor_t sensor_values) {
  uint16_t threshold_in_cm = 60;

  if ((sensor_values.left <= threshold_in_cm) || (sensor_values.right <= threshold_in_cm) ||
      (sensor_values.front <= threshold_in_cm) || (sensor_values.back <= threshold_in_cm)) {
    gpio__reset(board_io__get_led3());
  } else {
    gpio__set(board_io__get_led3());
  }
}

<<<<<<< HEAD
void led_handler__diagnostic_test_object_detection_leds_for_each_sensor(sensor_t sensor_values) {
=======
void led_handler__diagnostic_test_object_detection_leds_for_each_sensor(uint16_t left_sensor, uint16_t right_sensor,
                                                                        uint16_t front_sensor, uint16_t back_sensor) {
>>>>>>> sensor module changes
  uint16_t threshold_in_cm = 110;

  if (sensor_values.left <= threshold_in_cm) {
    gpio__reset(board_io__get_led3());
  } else {
    gpio__set(board_io__get_led3());
  }

  if (sensor_values.front <= threshold_in_cm) {
    gpio__reset(board_io__get_led2());
  } else {
    gpio__set(board_io__get_led2());
  }

  if (sensor_values.back <= threshold_in_cm) {
    gpio__reset(board_io__get_led1());
  } else {
    gpio__set(board_io__get_led1());
  }

  if (sensor_values.right <= threshold_in_cm) {
    gpio__reset(board_io__get_led0());
  } else {
    gpio__set(board_io__get_led0());
  }
}