#include "motor_logic.h"

#include "board_io.h"
#include "delay.h"
#include "gpio.h"
#include "speed_sensor.h"
#include "stdio.h"

static uint32_t reverse_counter = 0;
static bool reverse_flag = 0;

static float pwm_reverse = 14.07;
static float pwm_neutral = 15;
static float pwm_forward_default_low = 15.8;
static float pwm_forward_default_high = 18;
static float pwm_forward = 15.8;

static float target_speed_mph, actual_speed_mph;

static float low_speed_mph = 4;
static float medium_speed_mph = 8;

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

static void apply_brake(uint8_t brake_count) {
  switch (brake_count) {
  case 0:
  case 1:
    pwm1__set_duty_cycle(PWM_MOTOR, 10);
    break;
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
    pwm1__set_duty_cycle(PWM_MOTOR, 15);
    break;
  default:
    pwm1__set_duty_cycle(PWM_MOTOR, 15);
    break;
  }
}

static float maintain_speed(float actual_speed, float target_speed) {

  if (target_speed > actual_speed) {
    pwm_forward += 0.002;
  } else {
    pwm_forward -= 0.004;
  }

  // if ((actual_speed - target_speed_mph) > 3) {
  //   pwm_forward = pwm_forward_default_low;
  //   static uint8_t count = 0;
  //   if (count == 0) {
  //     count++;
  //     return 10.0f;
  //     // apply_brake(count++);
  //   } else {
  //     return pwm_forward_default_low;
  //     count = 0;
  //   }
  // }

  if ((actual_speed - target_speed_mph) > 4) {
    pwm_forward = pwm_forward_default_low;
    return 10.0f;
  }

  if (pwm_forward > pwm_forward_default_high) {
    pwm_forward = pwm_forward_default_high;
  }
  if (pwm_forward < pwm_forward_default_low) {
    pwm_forward = pwm_forward_default_low;
  }

  return pwm_forward;
}

static void dc_motor_forward(int16_t motor_speed) {
  reverse_counter = 0;
  reverse_flag = false;

  switch (motor_speed) {
  case 1:
    target_speed_mph = low_speed_mph;
    break;

  case 2:
  default:
    target_speed_mph = medium_speed_mph;
    break;
  }

  actual_speed_mph = get_mph();

  float pwm = maintain_speed(actual_speed_mph, target_speed_mph);

  pwm1__set_duty_cycle(PWM_MOTOR, pwm);
  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__reset(board_io__get_led3());
}

static void dc_motor_reverse(int16_t motor_speed) {
  pwm_forward = pwm_forward_default_low;
  reverse_flag = true;

  if (reverse_counter == 0) {
    pwm1__set_duty_cycle(PWM_MOTOR, 10);
  } else if (reverse_counter <= 5) {
    pwm1__set_duty_cycle(PWM_MOTOR, 15);
  } else {
    pwm1__set_duty_cycle(PWM_MOTOR, pwm_reverse);
    gpio__reset(board_io__get_led1());
    gpio__set(board_io__get_led2());
    gpio__set(board_io__get_led3());
  }

  reverse_counter++;
}

void dc_init(int call_back) {
  if (0 == call_back)
    pwm1__set_duty_cycle(PWM_MOTOR, 2);
  else if (1 == call_back)
    pwm1__set_duty_cycle(PWM_MOTOR, 1.5);
  else
    pwm1__set_duty_cycle(PWM_MOTOR, 1);
}

static void dc_motor_stop(int16_t motor_speed) {
  pwm_forward = pwm_forward_default_low;
  reverse_counter = 0;
  reverse_flag = false;

  pwm1__set_duty_cycle(PWM_MOTOR, pwm_neutral);
  gpio__set(board_io__get_led1());
  gpio__reset(board_io__get_led2());
  gpio__set(board_io__get_led3());
}

float motor_speed_with_direction(void) {
  float speed = get_mph();
  if (reverse_flag) {
    speed *= -1;
  }
  return speed;
}

float get_pwm_forward(void) { return pwm_forward; }

void control_motor_speed(int16_t motor_speed) {
  static uint64_t cb_count = 0;
  static uint8_t brake_count = 0;
  if (motor_speed == 0) {
    dc_motor_stop(motor_speed);
    cb_count = 0;
  } else if (motor_speed > 0) {
    // if (cb_count > 50 && cb_count < 150) {
    //   apply_brake(brake_count++);
    // } else if (cb_count > 150) {
    //   brake_count = 0;
    //   cb_count = 0;
    // } else {
    dc_motor_forward(motor_speed);
    // }
    cb_count++;
  } else {
    dc_motor_reverse(motor_speed);
    cb_count = 0;
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