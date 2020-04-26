#pragma once

#include "project.h"
#include "pwm1.h"

#define TURN_LEFT_90_DEGREES 11
#define TURN_RIGHT_90_DEGREES 19
#define TURN_LEFT_20_DEGREES 13.89
#define TURN_RIGHT_20_DEGREES 16.11
#define STEER_STRAIGHT 15

#define PWM_MOTOR PWM1__2_1
#define PWM_SERVO PWM1__2_0

typedef enum terrain_type { UPHILL = 0, FLAT, DOWNHILL } terrain_type_t;

// void motor_logic(dbc_DRIVER_STEER_SPEED_s *steer_data);
// void control_motor_steer(DRIVER_STEER_direction_e motor_steer);
// void control_motor_speed(int16_t motor_speed);
// void rc_car_stop_state(void);
// float motor_speed_with_direction(void);
// float get_pwm_forward(void);

void init_pwm(void);

void motor_logic(dbc_DRIVER_STEER_SPEED_s *steer_data);

void control_motor_steer(DRIVER_STEER_direction_e motor_steer);

void control_motor_speed(int16_t motor_speed);

void accelerate_forward_mph(double target_speed);

terrain_type_t get_terrain_data(void);

void stop_car(void);

void accelerate_reverse_mph(double target_speed);

void drive_motor(float pwm_value);