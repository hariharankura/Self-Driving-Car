#pragma once

#include "stdint.h"

void button_interrupt(void);

void initialize_speed_sensor_interrupt(void);
uint32_t get_rpm(void);
float get_mph(void);
void clear_rotations_in_windowtime(void);
uint16_t get_windowtime(void);