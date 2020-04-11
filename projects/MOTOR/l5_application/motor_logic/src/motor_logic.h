#pragma once

#define DUTYCYCLE_HARDLEFT 10
#define DUTYCYCLE_HARDRIGHT 20
#define DUTYCYCLE_SOFTLEFT 13
#define DUTYCYCLE_SOFTRIGHT 17
#define DUTYCYCLE_STRAIGHT 15

#define PWM_MOTOR PWM1__2_1
#define PWM_SERVO PWM1__2_0

#include "project.h"

void init_pwm(void);
void motor_logic(dbc_DRIVER_STEER_SPEED_s *steer_data);
void set_onboard_leds(void);