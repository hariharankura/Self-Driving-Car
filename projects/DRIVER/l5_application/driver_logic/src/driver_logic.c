#include "driver_logic.h"
#include "driving_algo.h"
#include "sjvalley_lcd.h"
#include <stdio.h>

static bool CAR_IN_START_MODE = false;
static dbc_MOTOR_SPEED_s car_speed = {};

static const dbc_DRIVER_STEER_SPEED_s STOP_COMMAND = {{}, DRIVER_STEER_direction_STRAIGHT, 0};

dbc_DRIVER_STEER_SPEED_s driver_logic__get_motor_command(void) {
  dbc_DRIVER_STEER_SPEED_s current_state;
  static dbc_DRIVER_STEER_SPEED_s last_state = {{0}, 0, 0};
  if (CAR_IN_START_MODE == true) {
    current_state = driving_algo__compute_heading();
    if (current_state.DRIVER_STEER_move_speed == DRIVER_STEER_move_REVERSE_at_SPEED &&
        last_state.DRIVER_STEER_move_speed == DRIVER_STEER_move_FORWARD_at_SPEED) {
      current_state.DRIVER_STEER_move_speed = DRIVER_STEER_move_STOP;
    }
  } else {
    current_state = STOP_COMMAND;
  }
  last_state = current_state;
  return current_state;
}

void driver_logic__set_car_mode(dbc_CAR_ACTION_s car_action) {
  if (car_action.CAR_ACTION_cmd > 0) {
    CAR_IN_START_MODE = true;
  } else {
    CAR_IN_START_MODE = false;
  }
}

void driver_logic__set_car_current_speed(dbc_MOTOR_SPEED_s l_car_speed) { car_speed = l_car_speed; }

void driver_logic__print_on_lcd_current_car_speed(void) {
  char car_speed_info_string[20] = {};
  snprintf(car_speed_info_string, 20, "SPEED=%f", car_speed.MOTOR_SPEED_info);
  sjvalley_lcd__send_line(1, car_speed_info_string);
}