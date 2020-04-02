#include "motor_logic.h"
#include "gpio.h"
#include "stdio.h"

gpio_s left_led, right_led, fwd_led, rev_led;

void init_led(void) {
  left_led = gpio__construct_as_output(GPIO__PORT_1, 18);
  right_led = gpio__construct_as_output(GPIO__PORT_1, 24);
  fwd_led = gpio__construct_as_output(GPIO__PORT_1, 26);
  rev_led = gpio__construct_as_output(GPIO__PORT_2, 3);
}

void motor_steer_logic(dbc_DRIVER_STEER_SPEED_s *steer_data) {

  if (steer_data->DRIVER_STEER_direction < 0) {
    gpio__reset(left_led);
    gpio__set(right_led);
    printf("Direction: LEFT\n");
  } else if (steer_data->DRIVER_STEER_direction > 0) {
    gpio__set(left_led);
    gpio__reset(right_led);
    printf("Direction: RIGHT\n");
  } else {
    gpio__set(left_led);
    gpio__set(right_led);
    printf("Direction: STRAIGHT\n");
  }

  if (steer_data->DRIVER_STEER_move_speed > 0) {
    gpio__reset(fwd_led);
    gpio__set(rev_led);
    printf("Move: FORWARD\n\n");
  } else if (steer_data->DRIVER_STEER_move_speed < 0) {
    gpio__set(fwd_led);
    gpio__reset(rev_led);
    printf("Move: REVERSE\n\n");
  } else {
    gpio__set(fwd_led);
    gpio__set(rev_led);
    printf("Move: STOP\n\n");
  }
}