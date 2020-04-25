#pragma once
#include "project.h"

static const uint16_t THRESHOLD_VERY_NEAR = 60;
static const uint16_t THRESHOLD_NEAR = 90;
static const uint16_t FRONT_BACK_THRESHOLD_VERY_NEAR = 100;
static const uint16_t FRONT_BACK_THRESHOLD_NEAR = 180;
static const int8_t DRIVER_STEER_move_FORWARD_at_SPEED = 2;
static const int8_t DRIVER_STEER_move_FORWARD_at_LOW_SPEED = 1;
static const int8_t DRIVER_STEER_move_REVERSE_at_SPEED = -2;
static const int8_t DRIVER_STEER_move_STOP = 0;

bool obstacle_avoidance__is_required();
void obstacle_avoidance__process_ultrasonic_sensors_data(const dbc_SENSOR_USONARS_s l_sensor_data);
void obstacle_avoidance__get_direction(dbc_DRIVER_STEER_SPEED_s *motor_info);

void obstacle_avoidance__print_debug_data(void);
