#pragma once

#include "project.h"

void driver_logic__process_ultrasonic_sensors_data(dbc_SENSOR_USONARS_s sensor_data);
void driver_logic__process_geo_compass_data(const dbc_GEO_COMPASS_s compass_angle);
dbc_DRIVER_STEER_SPEED_s driver_logic__get_motor_command(void);

dbc_GEO_COMPASS_s driver_logic__get_geo_compass_data(void);
dbc_SENSOR_USONARS_s driver_logic__get_ultrasonic_sensors_data(void);

const static uint16_t threshold_ultrasonic_sensor = 30;
static const int8_t DRIVER_STEER_move_FORWARD_at_SPEED = 5;
static const int8_t DRIVER_STEER_move_REVERSE_at_SPEED = -5;
static const int8_t DRIVER_STEER_move_STOP = 0;
