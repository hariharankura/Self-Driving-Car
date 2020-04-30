#pragma once

#include <stdint.h>

void initialize_speed_sensor_and_test_button_interrupts(void);
uint32_t get_rpm(void);
float get_mph(void);
void clear_rotations_in_windowtime(void);