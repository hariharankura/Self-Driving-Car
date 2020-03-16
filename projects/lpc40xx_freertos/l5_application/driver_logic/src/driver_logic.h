#pragma once

#include "project.h"

void driver_logic__process_ultrasonic_sensors_data(dbc_SENSOR_USONARS_s sensor_data);
bool driver_logic__check_if_steer_data_to_send(dbc_DRIVER_STEER_s *steer_info);
bool driver_logic__check_if_speed_data_to_send(dbc_MOTOR_SPEED_s *speed_info);