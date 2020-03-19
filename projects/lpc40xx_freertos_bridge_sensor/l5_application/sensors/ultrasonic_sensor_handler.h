#pragma once

#include <stdint.h>

void ultrasonic_sensor_handler__initialize_sensors(void);

uint16_t ultrasonic_sensor_handler__convert_12_bit_adc_value_to_cm(uint16_t adc_value);

uint16_t ultrasonic_sensor_handler__get_sensor_value_left(void);

uint16_t ultrasonic_sensor_handler__get_sensor_value_right(void);

uint16_t ultrasonic_sensor_handler__get_sensor_value_front(void);

uint16_t ultrasonic_sensor_handler__get_sensor_value_back(void);