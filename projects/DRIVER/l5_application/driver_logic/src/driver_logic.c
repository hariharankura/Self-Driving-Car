#include "driver_logic.h"
#include "driving_algo.h"
#include <stdio.h>
#include <string.h>

static bool CAR_IN_START_MODE = false;

static const dbc_DRIVER_STEER_SPEED_s STOP_COMMAND = {{}, DRIVER_STEER_direction_STRAIGHT, 0};

dbc_DRIVER_STEER_SPEED_s driver_logic__get_motor_command(void) {
  // return driving_algo__compute_heading();
  if (CAR_IN_START_MODE == true) {
    // printf("Algo\n");
    return driving_algo__compute_heading();
  } else {
    // printf("Command STop\n");
    return STOP_COMMAND;
  }
}

void driver_logic__set_car_mode(dbc_CAR_ACTION_s car_action) {
  // printf("CMD = %d\n", car_action.CAR_ACTION_cmd);
  if (car_action.CAR_ACTION_cmd > 0) {
    // printf("STart\n");
    CAR_IN_START_MODE = true;
  } else {
    // printf("Stop\n");
    CAR_IN_START_MODE = false;
  }
}
