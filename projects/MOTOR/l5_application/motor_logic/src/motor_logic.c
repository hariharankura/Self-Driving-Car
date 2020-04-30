#include "motor_logic.h"

#include <stdio.h>

#include "board_io.h"
#include "delay.h"
#include "gpio.h"
#include "speed_sensor.h"

static const float PWM_FORWARD_DEFAULT_LOW = 15.8;
static const float PWM_FORWARD_DEFAULT_HIGH = 18.0;
static const float PWM_NEUTRAL = 15.0;
static const float PWM_REVERSE = 14.07;

static uint32_t reverse_counter = 0;
static bool reverse_flag = 0;

static float pwm_forward = 15.8;

static float target_speed_mph, current_speed_mph;

void init_pwm(void) {
  gpio__construct_as_output(GPIO__PORT_2, 0);
  gpio__construct_as_output(GPIO__PORT_2, 1);
  gpio__construct_with_function(GPIO__PORT_2, 0, GPIO__FUNCTION_1);
  gpio__construct_with_function(GPIO__PORT_2, 1, GPIO__FUNCTION_1);
  pwm1__init_single_edge(100);
}

void rc_car_stop_state(void) {
  control_motor_steer(DRIVER_STEER_direction_STRAIGHT);
  control_motor_speed(0);
}

void motor_logic(dbc_DRIVER_STEER_SPEED_s *steer_data) {
  control_motor_steer(steer_data->DRIVER_STEER_direction);
  control_motor_speed(steer_data->DRIVER_STEER_move_speed);
}

static float maintain_speed() {
  current_speed_mph = get_mph();

  static uint16_t wait_count = 50;
  if ((current_speed_mph - target_speed_mph) > 1.5f) { // Brake logic
    pwm_forward = PWM_FORWARD_DEFAULT_LOW;
    if (reverse_flag == true) {
      wait_count = 0;
    }
    if (wait_count < 15) {
      wait_count++;
      return pwm_forward;
    }
    return 10.0f;
  }

  if (target_speed_mph > current_speed_mph) {
    pwm_forward += 0.002f;
  } else if (target_speed_mph < current_speed_mph) {
    pwm_forward -= 0.004f;
  }

  if (pwm_forward > PWM_FORWARD_DEFAULT_HIGH) {
    pwm_forward = PWM_FORWARD_DEFAULT_HIGH;
  } else if (pwm_forward < PWM_FORWARD_DEFAULT_LOW) {
    pwm_forward = PWM_FORWARD_DEFAULT_LOW;
  }

  return pwm_forward;
}

static void dc_motor_forward(int16_t motor_speed) {
  reverse_counter = 0;

  target_speed_mph = (float)motor_speed;

  float pwm = maintain_speed();
  pwm1__set_duty_cycle(PWM_MOTOR, pwm);

  reverse_flag = false;

  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__reset(board_io__get_led3());
}

static void dc_motor_reverse(int16_t motor_speed) {
  pwm_forward = PWM_FORWARD_DEFAULT_LOW;
  reverse_flag = true;

  if (reverse_counter <= 3) {
    pwm1__set_duty_cycle(PWM_MOTOR, 10);
  } else if (reverse_counter <= 5) {
    pwm1__set_duty_cycle(PWM_MOTOR, 15);
  } else {
    pwm1__set_duty_cycle(PWM_MOTOR, PWM_REVERSE);
    gpio__reset(board_io__get_led1());
    gpio__set(board_io__get_led2());
    gpio__set(board_io__get_led3());
  }
  reverse_counter++;
}

static void dc_motor_stop(int16_t motor_speed) {
  pwm_forward = PWM_FORWARD_DEFAULT_LOW;
  reverse_counter = 0;
  reverse_flag = false;

  pwm1__set_duty_cycle(PWM_MOTOR, PWM_NEUTRAL);
  gpio__set(board_io__get_led1());
  gpio__reset(board_io__get_led2());
  gpio__set(board_io__get_led3());
}

void control_motor_speed(int16_t motor_speed) {
  if (motor_speed == 0) {
    dc_motor_stop(motor_speed);
  } else if (motor_speed > 0) {
    dc_motor_forward(motor_speed);
  } else if (motor_speed < 0) {
    dc_motor_reverse(motor_speed);
  }
}

void control_motor_steer(DRIVER_STEER_direction_e motor_steer) {
  switch (motor_steer) {
  case DRIVER_STEER_direction_HARD_LEFT:
    pwm1__set_duty_cycle(PWM_SERVO, TURN_LEFT_90_DEGREES);
    break;
  case DRIVER_STEER_direction_SOFT_LEFT:
    pwm1__set_duty_cycle(PWM_SERVO, TURN_LEFT_20_DEGREES);
    break;
  case DRIVER_STEER_direction_STRAIGHT:
    pwm1__set_duty_cycle(PWM_SERVO, STEER_STRAIGHT);
    break;
  case DRIVER_STEER_direction_SOFT_RIGHT:
    pwm1__set_duty_cycle(PWM_SERVO, TURN_RIGHT_20_DEGREES);
    break;
  case DRIVER_STEER_direction_HARD_RIGHT:
    pwm1__set_duty_cycle(PWM_SERVO, TURN_RIGHT_90_DEGREES);
    break;
  default:
    break;
  }
}

float motor_speed_with_direction(void) {
  float speed = get_mph();
  if (reverse_flag) {
    speed *= -1;
  }
  return speed;
}

float get_pwm_forward(void) { return pwm_forward; }