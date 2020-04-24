#pragma once

#include "bridge_buffer.h"
#include "gpio.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "project.h"
#include "sl_string.h"
=======
>>>>>>> sensor module changes
=======
#include "sl_string.h"
>>>>>>> new bridge changes
#include "uart.h"
#include "ultrasonic_sensor_handler.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern float debug_motor_speed;
extern float debug_motor_speed_pwm;
extern float debug_geo_compass_current_heading;
extern float debug_geo_compass_destination_heading;
extern float debug_geo_compass_distance;
extern int debug_steer_move_speed;
extern DRIVER_STEER_direction_e debug_steer_direction;

typedef struct {
  float latitude;
  float longitude;
  char latitude_direction;
  char longitude_direction;

} gps_data;

typedef struct {

  // user commands
  uint8_t car_action;
  uint8_t headlight_command;
  uint8_t test_button;

  // ultrasonic
  uint16_t ultrasonic_left;
  uint16_t ultrasonic_right;
  uint16_t ultrasonic_front;
  uint16_t ultrasonic_back;

  // motor speed
  float motor_speed;

  // steer speed
  int16_t steer_move_speed;
  DRIVER_STEER_direction_e steer_direction;

  // Geo Compass
  uint16_t compass_current_heading;
  uint16_t compass_desitination_heading;
  uint8_t compass_distance;

} debug_information;

void bridge_controller_handler__initialize_bluetooth_module(void);
void bridge_controller_handler__convert_gps_data_to_degrees(gps_data *data);
void bridge_controller_handler__assign_lat_long_direction(gps_data *data);
void bridge_controller_handler__parse_gps_data(const char *input_buffer, float *latitude, float *longitude);
void bridge_controller_handler__get_data_from_uart(void);
bool bridge_controller_handler__buffer_has_gps_message(const char *array, int *message_count);
void bridge_controller_handler__get_single_gps_message(char *line);
bool bridge_controller_handler__get_gps_message_from_buffer(char *temp_buffer);
void bridge_controller_handler__get_gps_coordinates(float *latitude, float *longitude);
<<<<<<< HEAD
<<<<<<< HEAD
void bridge_controller_handler__get_destination_coordinates(float *latitude, float *longitude);
bool bridge_controller_handler__buffer_has_start_stop_message();
bool bridge_controller_handler__get_start_stop_condition();
void bridge_controller_handler__send_debug_info(void);
=======

void bridge_controller_handler__get_destination_coordinates(float *latitude, float *longitude);
>>>>>>> sensor module changes
=======
void bridge_controller_handler__get_destination_coordinates(float *latitude, float *longitude);
bool bridge_controller_handler__buffer_has_start_stop_message();
bool bridge_controller_handler__get_start_stop_condition();
>>>>>>> new bridge changes
