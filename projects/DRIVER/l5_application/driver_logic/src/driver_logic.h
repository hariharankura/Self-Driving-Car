#pragma once

#include "project.h"

void driver_logic__process_ultrasonic_sensors_data(dbc_SENSOR_USONARS_s sensor_data);
dbc_DRIVER_STEER_SPEED_s driver_logic__get_motor_command(void);