#pragma once

#include "project.h"

dbc_DRIVER_STEER_SPEED_s driver_logic__get_motor_command(void);

void driver_logic__set_car_mode(dbc_CAR_ACTION_s car_action);