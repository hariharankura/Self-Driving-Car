#include "unity.h"

#include "Mockboard_io.h"
#include "Mockgpio.h"

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
  gpio_s led3;

  board_io__get_led3_ExpectAndReturn(led3);
  gpio__set_Expect(led3);
  led_handler__diagnostic_test_object_detection_led3(300, 300, 300, 300);

  board_io__get_led3_ExpectAndReturn(led3);
  gpio__reset_Expect(led3);
  led_handler__diagnostic_test_object_detection_led3(30, 30, 30, 30);

  board_io__get_led3_ExpectAndReturn(led3);
  gpio__reset_Expect(led3);
  led_handler__diagnostic_test_object_detection_led3(49, 300, 300, 300);

  board_io__get_led3_ExpectAndReturn(led3);
  gpio__reset_Expect(led3);
  led_handler__diagnostic_test_object_detection_led3(300, 300, 60, 300);

  board_io__get_led3_ExpectAndReturn(led3);
  gpio__set_Expect(led3);
  led_handler__diagnostic_test_object_detection_led3(300, 61, 300, 300);
}

void test_led_handler__diagnostic_test_object_detection_leds_for_each_sensor(void) {
  gpio_s led0, led1, led2, led3;

  board_io__get_led3_ExpectAndReturn(led3);
  gpio__set_Expect(led3);

  board_io__get_led2_ExpectAndReturn(led2);
  gpio__set_Expect(led2);

  board_io__get_led1_ExpectAndReturn(led1);
  gpio__set_Expect(led1);

  board_io__get_led0_ExpectAndReturn(led0);
  gpio__set_Expect(led0);
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor(300, 300, 300, 300);

  board_io__get_led3_ExpectAndReturn(led3);
  gpio__reset_Expect(led3);

  board_io__get_led2_ExpectAndReturn(led2);
  gpio__reset_Expect(led2);

  board_io__get_led1_ExpectAndReturn(led1);
  gpio__reset_Expect(led1);

  board_io__get_led0_ExpectAndReturn(led0);
  gpio__reset_Expect(led0);
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor(60, 60, 60, 60);

  board_io__get_led3_ExpectAndReturn(led3);
  gpio__set_Expect(led3);

  board_io__get_led2_ExpectAndReturn(led2);
  gpio__reset_Expect(led2);

  board_io__get_led1_ExpectAndReturn(led1);
  gpio__set_Expect(led1);

  board_io__get_led0_ExpectAndReturn(led0);
  gpio__set_Expect(led0);
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor(150, 117, 110, 120);
}