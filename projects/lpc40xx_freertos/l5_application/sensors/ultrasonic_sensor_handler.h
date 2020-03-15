#pragma once

#include <stdint.h>

// initialization and setup for ultrasonic sensors (using ADC driver)
// functions to retrieve data from each ultrasonic sensor

void ultrasonic_sensor_handler__initialize_sensors(void);

uint16_t ultrasonic_sensor_handler__get_sensor_value_left(void);

uint16_t ultrasonic_sensor_handler__get_sensor_value_right(void);

uint16_t ultrasonic_sensor_handler__get_sensor_value_front(void);

uint16_t ultrasonic_sensor_handler__get_sensor_value_back(void);