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