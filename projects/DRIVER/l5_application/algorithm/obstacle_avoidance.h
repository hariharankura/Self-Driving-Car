#pragma once
#include "project.h"

static const uint16_t THRESHOLD_VERY_NEAR = 20;
static const uint16_t THRESHOLD_NEAR = 40;
static const int8_t DRIVER_STEER_move_FORWARD_at_SPEED = 5;
static const int8_t DRIVER_STEER_move_REVERSE_at_SPEED = -5;
static const int8_t DRIVER_STEER_move_STOP = 0;

bool obstacle_avoidance__is_required();
void obstacle_avoidance__process_ultrasonic_sensors_data(const dbc_SENSOR_USONARS_s l_sensor_data);
void obstacle_avoidance__get_direction(dbc_DRIVER_STEER_SPEED_s *motor_info);
