#include "driver_logic.h"
#include "driving_algo.h"
#include "project.h"
#include <stdio.h>
#include <string.h>

static bool CAR_IN_START_MODE = false;

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
