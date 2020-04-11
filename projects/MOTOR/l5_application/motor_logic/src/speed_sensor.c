#include "speed_sensor.h"
#include "board_io.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"

#include "stdio.h"

const float pi = 3.1416;
const float wheel_diameter_in_cm = 10;
const float ms_to_mph = 2.237;
const float cm_to_meters = 0.01;

uint8_t windowtime_in_seconds = 5;
uint32_t rotations_in_windowtime = 0, recorded_rotations_in_windowtime = 0;
uint32_t rpm, recorded_rpm;
uint32_t mph, recorded_mph;

bool window_elapsed = false;

void button_interrupt(void) {
  if (((LPC_GPIOINT->IO0IntStatR >> 29) & (1 << 0)) == 1) {
    rotations_in_windowtime++;
    LPC_GPIOINT->IO0IntClr = (1 << 29);
  }
}

void init_speed_sensor(void) {

  gpio__construct_as_input(0, 29);

  gpio_enable_interrupt(29);

  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, button_interrupt, NULL);

  gpio__set(board_io__get_led0());
  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__set(board_io__get_led3());
}

uint32_t get_rpm(void) {

  rpm = (rotations_in_windowtime * (60 / windowtime_in_seconds));

  return rpm;
}

uint32_t get_mph(void) {

  mph = (pi * wheel_diameter_in_cm) * rpm * cm_to_meters * ms_to_mph;

  return mph;
}

void clear_rotations_in_windowtime(void) {

  recorded_mph = mph;
  recorded_rpm = rpm;
  recorded_rotations_in_windowtime = rotations_in_windowtime;
  print_recorded_data();
  rotations_in_windowtime = 0;
}

uint8_t get_windowtime(void) { return windowtime_in_seconds; }

void print_recorded_data(void) {

  printf("rotations = %ld\nrpm = %ld\nmph = %ld\n", recorded_rotations_in_windowtime, recorded_rpm, recorded_mph);
}