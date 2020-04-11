#pragma once

#include "stdint.h"

void button_interrupt(void);

void init_speed_sensor(void);
uint32_t get_rpm(void);
uint32_t get_mph(void);
void clear_rotations_in_windowtime(void);
uint8_t get_windowtime(void);
void print_recorded_data(void);
