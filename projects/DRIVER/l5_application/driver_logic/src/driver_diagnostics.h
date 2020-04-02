#pragma once
#include "project.h"

void diagnostics_led_init(void);

void light_up_direction_led(dbc_DRIVER_STEER_SPEED_s motor_command_struct);

void clear_all_diagnostics_led(void);