#include "driver_logic.h"
#include "driving_algo.h"
#include <stdio.h>
#include <string.h>

static dbc_SENSOR_USONARS_s ultrasonic_sensor_data;
static dbc_GEO_COMPASS_s current_and_destination_heading_angle;

void driver_logic__process_ultrasonic_sensors_data(const dbc_SENSOR_USONARS_s sensor_data) {
  ultrasonic_sensor_data = sensor_data;
}

dbc_SENSOR_USONARS_s driver_logic__get_ultrasonic_sensors_data(void) { return ultrasonic_sensor_data; }

void driver_logic__process_geo_compass_data(const dbc_GEO_COMPASS_s compass_angle) {
  current_and_destination_heading_angle = compass_angle;
}

dbc_GEO_COMPASS_s driver_logic__get_geo_compass_data(void) { return current_and_destination_heading_angle; }

dbc_DRIVER_STEER_SPEED_s driver_logic__get_motor_command(void) { return driving_algo__compute_heading(); }
