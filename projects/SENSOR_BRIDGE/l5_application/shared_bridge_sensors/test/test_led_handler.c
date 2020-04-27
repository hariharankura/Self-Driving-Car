#include "unity.h"

#include "Mockboard_io.h"
#include "Mockgpio.h"
#include "Mockultrasonic_sensor_handler.h"

#include "led_handler.h"

void test_led_handler__turn_off_all_sjtwo_leds(void) {
  gpio_s led0, led1, led2, led3;
  board_io__get_led0_ExpectAndReturn(led0);
  gpio__set_Expect(led0);
  board_io__get_led1_ExpectAndReturn(led1);
  gpio__set_Expect(led1);
  board_io__get_led2_ExpectAndReturn(led2);
  gpio__set_Expect(led2);
  board_io__get_led3_ExpectAndReturn(led3);
  gpio__set_Expect(led3);

  led_handler__turn_off_all_sjtwo_leds();
}

void test_led_handler__diagnostic_test_object_detection_led3(void) {
  sensor_t sensor_values;
  gpio_s led3;

  sensor_values.left = 300;
  sensor_values.right = 300;
  sensor_values.front = 300;
  sensor_values.back = 300;
  board_io__get_led3_ExpectAndReturn(led3);
  gpio__set_Expect(led3);
  led_handler__diagnostic_test_object_detection_led3(sensor_values);

  sensor_values.left = 30;
  sensor_values.right = 30;
  sensor_values.front = 30;
  sensor_values.back = 30;
  board_io__get_led3_ExpectAndReturn(led3);
  gpio__reset_Expect(led3);
  led_handler__diagnostic_test_object_detection_led3(sensor_values);

  sensor_values.left = 49;
  sensor_values.right = 300;
  sensor_values.front = 300;
  sensor_values.back = 300;
  board_io__get_led3_ExpectAndReturn(led3);
  gpio__reset_Expect(led3);
  led_handler__diagnostic_test_object_detection_led3(sensor_values);

  sensor_values.left = 300;
  sensor_values.right = 300;
  sensor_values.front = 60;
  sensor_values.back = 300;
  board_io__get_led3_ExpectAndReturn(led3);
  gpio__reset_Expect(led3);
  led_handler__diagnostic_test_object_detection_led3(sensor_values);

  sensor_values.left = 300;
  sensor_values.right = 61;
  sensor_values.front = 300;
  sensor_values.back = 300;
  board_io__get_led3_ExpectAndReturn(led3);
  gpio__set_Expect(led3);
  led_handler__diagnostic_test_object_detection_led3(sensor_values);
}

void test_led_handler__diagnostic_test_object_detection_leds_for_each_sensor(void) {
  sensor_t sensor_values;
  gpio_s led0, led1, led2, led3;

  sensor_values.left = 300;
  sensor_values.right = 300;
  sensor_values.front = 300;
  sensor_values.back = 300;

  board_io__get_led3_ExpectAndReturn(led3);
  gpio__set_Expect(led3);

  board_io__get_led2_ExpectAndReturn(led2);
  gpio__set_Expect(led2);

  board_io__get_led1_ExpectAndReturn(led1);
  gpio__set_Expect(led1);

  board_io__get_led0_ExpectAndReturn(led0);
  gpio__set_Expect(led0);
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor(sensor_values);

  sensor_values.left = 60;
  sensor_values.right = 60;
  sensor_values.front = 60;
  sensor_values.back = 60;

  board_io__get_led3_ExpectAndReturn(led3);
  gpio__reset_Expect(led3);

  board_io__get_led2_ExpectAndReturn(led2);
  gpio__reset_Expect(led2);

  board_io__get_led1_ExpectAndReturn(led1);
  gpio__reset_Expect(led1);

  board_io__get_led0_ExpectAndReturn(led0);
  gpio__reset_Expect(led0);
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor(sensor_values);

  sensor_values.left = 150;
  sensor_values.right = 117;
  sensor_values.front = 110;
  sensor_values.back = 120;

  board_io__get_led3_ExpectAndReturn(led3);
  gpio__set_Expect(led3);

  board_io__get_led2_ExpectAndReturn(led2);
  gpio__reset_Expect(led2);

  board_io__get_led1_ExpectAndReturn(led1);
  gpio__set_Expect(led1);

  board_io__get_led0_ExpectAndReturn(led0);
  gpio__set_Expect(led0);
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor(sensor_values);
}