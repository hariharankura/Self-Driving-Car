#pragma once

#include "bridge_buffer.h"
#include "gpio.h"
#include "uart.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
  float latitude;
  float longitude;
  char latitude_direction;
  char longitude_direction;

} gps_data;

void bridge_controller_handler__initialize_bluetooth_module(void);
void bridge_controller_handler__convert_gps_data_to_degrees(gps_data *data);
void bridge_controller_handler__assign_lat_long_direction(gps_data *data);
void bridge_controller_handler__parse_gps_data(const char *input_buffer, float *latitude, float *longitude);
void bridge_controller_handler__get_data_from_uart(void);
bool bridge_controller_handler__buffer_has_gps_message(const char *array, int *message_count);
void bridge_controller_handler__get_single_gps_message(char *line);
bool bridge_controller_handler__get_gps_message_from_buffer(char *temp_buffer);
void bridge_controller_handler__get_gps_coordinates(float *latitude, float *longitude);

void bridge_controller_handler__get_destination_coordinates(float *latitude, float *longitude);

bool bridge_controller_handler__get_start_stop_condition();