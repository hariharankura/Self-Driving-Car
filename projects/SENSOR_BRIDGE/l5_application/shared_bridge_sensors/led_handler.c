#include "led_handler.h"
#include "board_io.h"
#include "gpio.h"

void led_handler__turn_off_all_sjtwo_leds(void) {
  gpio__set(board_io__get_led0());
  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__set(board_io__get_led3());
}