#pragma once

#include <stdint.h>

typedef struct {
  uint16_t left;
  uint16_t right;
  uint16_t front;
  uint16_t back;
} sensor_t;

void ultrasonic_sensor_handler__initialize_sensors(void);

void ultrasonic_sensor_handler__get_all_sensor_values(sensor_t *sensor_values);

void ultrasonic_sensor_handler__set_all_sensor_values(uint16_t left, uint16_t right, uint16_t front, uint16_t back);

uint16_t ultrasonic_sensor_handler__get_sensor_value_left(void);

uint16_t ultrasonic_sensor_handler__get_sensor_value_right(void);

uint16_t ultrasonic_sensor_handler__get_sensor_value_front(void);

uint16_t ultrasonic_sensor_handler__get_sensor_value_back(void);

uint16_t ultrasonic_sensor_handler__get_filtered_sensor_value_left(void);

uint16_t ultrasonic_sensor_handler__get_filtered_sensor_value_right(void);

uint16_t ultrasonic_sensor_handler__get_filtered_sensor_value_front(void);

uint16_t ultrasonic_sensor_handler__get_filtered_sensor_value_back(void);