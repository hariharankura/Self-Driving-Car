#pragma once

#include "project.h"

dbc_DRIVER_STEER_SPEED_s driver_logic__get_motor_command(void);
void driver_logic__set_car_mode(dbc_CAR_ACTION_s car_action);
void driver_logic__set_car_current_speed(dbc_MOTOR_SPEED_s l_car_speed);
void driver_logic__print_on_lcd_current_car_speed(void);
