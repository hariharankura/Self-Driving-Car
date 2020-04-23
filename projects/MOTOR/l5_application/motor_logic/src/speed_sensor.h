#pragma once

#include "stdint.h"

void button_interrupt(void);

void initialize_test_button_and_speed_sensor_interrupts(void);
uint32_t get_rpm(void);
float get_mph(void);
void clear_rotations_in_windowtime(void);
uint8_t get_windowtime(void);
void print_recorded_data(void);
uint8_t get_motor_test_button_status(void);
void reset_motor_test_button_status(void);