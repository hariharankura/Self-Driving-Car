#include "motor_logic.h"
#include "board_io.h"
#include "delay.h"
#include "gpio.h"
#include "pwm1.h"
#include "stdio.h"

static uint8_t reverse_counter = 0;

void init_pwm(void) {
  gpio__construct_as_output(GPIO__PORT_2, 0);
  gpio__construct_as_output(GPIO__PORT_2, 1);
  gpio__construct_with_function(GPIO__PORT_2, 0, GPIO__FUNCTION_1);
  gpio__construct_with_function(GPIO__PORT_2, 1, GPIO__FUNCTION_1);
  pwm1__init_single_edge(100);
}

void control_motor_steer(DRIVER_STEER_direction_e motor_steer) {
  switch (motor_steer) {
  case DRIVER_STEER_direction_HARD_LEFT:
    pwm1__set_duty_cycle(PWM_SERVO, TURN_LEFT_90_DEGREES);
    gpio__reset(board_io__get_led3());
    gpio__set(board_io__get_led2());
    // printf("Direction: HARD_LEFT\n");
    break;
  case DRIVER_STEER_direction_SOFT_LEFT:
    pwm1__set_duty_cycle(PWM_SERVO, TURN_LEFT_20_DEGREES);
    gpio__reset(board_io__get_led3());
    gpio__set(board_io__get_led2());
    // printf("Direction: SOFT_LEFT\n");
    break;
  case DRIVER_STEER_direction_STRAIGHT:
    pwm1__set_duty_cycle(PWM_SERVO, STEER_STRAIGHT);
    gpio__set(board_io__get_led3());
    gpio__set(board_io__get_led2());
    // printf("Direction: STRAIGHT\n");
    break;
  case DRIVER_STEER_direction_SOFT_RIGHT:
    pwm1__set_duty_cycle(PWM_SERVO, TURN_RIGHT_20_DEGREES);
    gpio__set(board_io__get_led3());
    gpio__reset(board_io__get_led2());
    // printf("Direction: SOFT_RIGHT\n");
    break;
  case DRIVER_STEER_direction_HARD_RIGHT:
    pwm1__set_duty_cycle(PWM_SERVO, TURN_RIGHT_90_DEGREES);
    gpio__set(board_io__get_led3());
    gpio__reset(board_io__get_led2());
    // printf("Direction: HARD_RIGHT\n");
    break;
  default:
    break;
  }
}

static void dc_motor_forward(int16_t motor_speed) {
  reverse_counter = 0;
  float duty_cycle = 15.5 + 0.2 * motor_speed;
  pwm1__set_duty_cycle(PWM_MOTOR, duty_cycle);
  gpio__reset(board_io__get_led1());
  // printf("Speed msg: %d  Duty Cycle: %f\n\n", motor_speed, duty_cycle);
}

static void dc_motor_reverse(int16_t motor_speed) {
  float duty_cycle = 14.17;

  if (reverse_counter <= 3) {
    pwm1__set_duty_cycle(PWM_MOTOR, 13.7);
    // printf("REVERSE\n");
  } else if (reverse_counter <= 5) {
    pwm1__set_duty_cycle(PWM_MOTOR, 15);
    // printf("STOP\n");
  } else {
    pwm1__set_duty_cycle(PWM_MOTOR, duty_cycle);
    gpio__set(board_io__get_led1());
    // printf("REVERSE\n");
  }
  // printf("Speed msg: %d  Duty Cycle: %f\n\n", motor_speed, duty_cycle);
  reverse_counter++;
}

static void dc_motor_stop(int16_t motor_speed) {
  reverse_counter = 0;
  pwm1__set_duty_cycle(PWM_MOTOR, 15);
  gpio__set(board_io__get_led1());
  // printf("Speed msg: 0  Duty Cycle: 15\n\n");
}

void control_motor_speed(int16_t motor_speed) {

  if (motor_speed == 0) {
    dc_motor_stop(motor_speed);
  } else if (motor_speed > 0) {
    dc_motor_forward(motor_speed);
  } else {
    dc_motor_reverse(motor_speed);
  }
}

void rc_car_stop_state(void) {
  control_motor_steer(DRIVER_STEER_direction_STRAIGHT);
  control_motor_speed(0);
}

void motor_logic(dbc_DRIVER_STEER_SPEED_s *steer_data) {
  control_motor_steer(steer_data->DRIVER_STEER_direction);
  control_motor_speed(steer_data->DRIVER_STEER_move_speed);
}