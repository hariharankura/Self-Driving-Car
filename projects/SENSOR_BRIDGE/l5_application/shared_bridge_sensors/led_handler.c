#include "led_handler.h"
#include "board_io.h"
#include "gpio.h"

void led_handler__turn_off_all_sjtwo_leds(void) {
  gpio__set(board_io__get_led0());
  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__set(board_io__get_led3());
}

void led_handler__diagnostic_test_object_detection_led3(uint16_t left_sensor, uint16_t right_sensor,
                                                        uint16_t front_sensor, uint16_t back_sensor) {
  uint16_t threshold_in_cm = 60;

  if ((left_sensor <= threshold_in_cm) || (right_sensor <= threshold_in_cm) || (front_sensor <= threshold_in_cm) ||
      (back_sensor <= threshold_in_cm)) {
    gpio__reset(board_io__get_led3());
  } else {
    gpio__set(board_io__get_led3());
  }
}

void led_handler__diagnostic_test_object_detection_leds_for_each_sensor(uint16_t left_sensor, uint16_t right_sensor,
                                                                        uint16_t front_sensor, uint16_t back_sensor) {
  uint16_t threshold_in_cm = 110;

  if (left_sensor <= threshold_in_cm) {
    gpio__reset(board_io__get_led3());
  } else {
    gpio__set(board_io__get_led3());
  }

  if (front_sensor <= threshold_in_cm) {
    gpio__reset(board_io__get_led2());
  } else {
    gpio__set(board_io__get_led2());
  }

  if (back_sensor <= threshold_in_cm) {
    gpio__reset(board_io__get_led1());
  } else {
    gpio__set(board_io__get_led1());
  }

  if (right_sensor <= threshold_in_cm) {
    gpio__reset(board_io__get_led0());
  } else {
    gpio__set(board_io__get_led0());
  }
}