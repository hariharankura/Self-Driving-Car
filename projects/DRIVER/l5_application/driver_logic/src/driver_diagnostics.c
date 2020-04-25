#include "driver_diagnostics.h"
#include "gpio.h"
#include "obstacle_avoidance.h"

static gpio_s diag_LED0, diag_LED1, diag_LED2, diag_LED3, diag_LED4, diag_LED5, diag_LED6, diag_LED7;

void diagnostics_led_init(void) {
  // LED0 : P1.4
  diag_LED0 = gpio__construct_as_output(1, 4);
  gpio__set_function(diag_LED0, GPIO__FUNCITON_0_IO_PIN);

  // LED1 : P4.28
  diag_LED1 = gpio__construct_as_output(2, 2);
  gpio__set_function(diag_LED1, GPIO__FUNCITON_0_IO_PIN);

  // LED2 : P0.6
  diag_LED2 = gpio__construct_as_output(0, 6);
  gpio__set_function(diag_LED2, GPIO__FUNCITON_0_IO_PIN);

  // LED3 : P0.8
  diag_LED3 = gpio__construct_as_output(0, 8);
  gpio__set_function(diag_LED3, GPIO__FUNCITON_0_IO_PIN);

  // LED4 : P4.29
  diag_LED4 = gpio__construct_as_output(2, 4);
  gpio__set_function(diag_LED4, GPIO__FUNCITON_0_IO_PIN);

  // LED5 : P0.7
  diag_LED5 = gpio__construct_as_output(0, 7);
  gpio__set_function(diag_LED5, GPIO__FUNCITON_0_IO_PIN);

  // LED6 : P0.9
  diag_LED6 = gpio__construct_as_output(0, 9);
  gpio__set_function(diag_LED6, GPIO__FUNCITON_0_IO_PIN);

  // LED7 : P1.30
  diag_LED7 = gpio__construct_as_output(1, 30);
  gpio__set_function(diag_LED7, GPIO__FUNCITON_0_IO_PIN);
}

void light_up_direction_led(dbc_DRIVER_STEER_SPEED_s motor_command_struct) {
  if (motor_command_struct.DRIVER_STEER_direction == DRIVER_STEER_direction_STRAIGHT &&
      motor_command_struct.DRIVER_STEER_move_speed == DRIVER_STEER_move_FORWARD_at_SPEED) {
    clear_all_diagnostics_led();
    gpio__reset(diag_LED0);
  }

  else if (motor_command_struct.DRIVER_STEER_direction == DRIVER_STEER_direction_STRAIGHT &&
           motor_command_struct.DRIVER_STEER_move_speed == DRIVER_STEER_move_REVERSE_at_SPEED) {
    clear_all_diagnostics_led();
    gpio__reset(diag_LED4);
  }

  else if (motor_command_struct.DRIVER_STEER_direction == DRIVER_STEER_direction_HARD_LEFT) {
    clear_all_diagnostics_led();
    gpio__reset(diag_LED6);

  }

  else if (motor_command_struct.DRIVER_STEER_direction == DRIVER_STEER_direction_SOFT_LEFT &&
           motor_command_struct.DRIVER_STEER_move_speed == DRIVER_STEER_move_FORWARD_at_SPEED) {
    clear_all_diagnostics_led();
    gpio__reset(diag_LED7);
  }

  else if (motor_command_struct.DRIVER_STEER_direction == DRIVER_STEER_direction_SOFT_LEFT &&
           motor_command_struct.DRIVER_STEER_move_speed == DRIVER_STEER_move_REVERSE_at_SPEED) {
    clear_all_diagnostics_led();
    gpio__reset(diag_LED5);
  }

  else if (motor_command_struct.DRIVER_STEER_direction == DRIVER_STEER_direction_HARD_RIGHT) {
    clear_all_diagnostics_led();
    gpio__reset(diag_LED2);
  }

  else if (motor_command_struct.DRIVER_STEER_direction == DRIVER_STEER_direction_SOFT_RIGHT &&
           motor_command_struct.DRIVER_STEER_move_speed == DRIVER_STEER_move_FORWARD_at_SPEED) {
    clear_all_diagnostics_led();
    gpio__reset(diag_LED1);
  }

  else if (motor_command_struct.DRIVER_STEER_direction == DRIVER_STEER_direction_SOFT_RIGHT &&
           motor_command_struct.DRIVER_STEER_move_speed == DRIVER_STEER_move_REVERSE_at_SPEED) {
    clear_all_diagnostics_led();
    gpio__reset(diag_LED3);
  }
}

void clear_all_diagnostics_led(void) {
  gpio__set(diag_LED0);
  gpio__set(diag_LED1);
  gpio__set(diag_LED2);
  gpio__set(diag_LED3);
  gpio__set(diag_LED4);
  gpio__set(diag_LED5);
  gpio__set(diag_LED6);
  gpio__set(diag_LED7);
}