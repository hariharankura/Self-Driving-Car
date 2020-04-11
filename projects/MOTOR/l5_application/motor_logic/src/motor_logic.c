#include "motor_logic.h"
#include "board_io.h"
#include "delay.h"
#include "gpio.h"
#include "pwm1.h"
#include "stdio.h"

void init_pwm(void) {
  gpio__construct_as_output(GPIO__PORT_2, 0);
  gpio__construct_as_output(GPIO__PORT_2, 1);
  gpio__construct_with_function(GPIO__PORT_2, 0, GPIO__FUNCTION_1);
  gpio__construct_with_function(GPIO__PORT_2, 1, GPIO__FUNCTION_1);
  pwm1__init_single_edge(1000);
}

static void control_motor_steer(DRIVER_STEER_direction_e motor_steer) {
  switch (motor_steer) {
  case DRIVER_STEER_direction_HARD_LEFT:
    pwm1__set_duty_cycle(PWM_SERVO, DUTYCYCLE_HARDLEFT);
    printf("Direction: HARD_LEFT\n");
    break;
  case DRIVER_STEER_direction_SOFT_LEFT:
    pwm1__set_duty_cycle(PWM_SERVO, DUTYCYCLE_SOFTLEFT);
    printf("Direction: SOFT_LEFT\n");
    break;
  case DRIVER_STEER_direction_STRAIGHT:
    pwm1__set_duty_cycle(PWM_SERVO, DUTYCYCLE_STRAIGHT);
    printf("Direction: STRAIGHT\n");
    break;
  case DRIVER_STEER_direction_SOFT_RIGHT:
    pwm1__set_duty_cycle(PWM_SERVO, DUTYCYCLE_SOFTRIGHT);
    printf("Direction: SOFT_RIGHT\n");
    break;
  case DRIVER_STEER_direction_HARD_RIGHT:
    pwm1__set_duty_cycle(PWM_SERVO, DUTYCYCLE_HARDRIGHT);
    printf("Direction: HARD_RIGHT\n");
    break;
  default:
    break;
  }
}

static void control_motor_speed(int16_t motor_speed) {
  pwm1__set_duty_cycle(PWM1__2_1, 15);
  delay__ms(100);
  float duty_cycle = 15 + 0.5 * motor_speed;
  pwm1__set_duty_cycle(PWM_MOTOR, duty_cycle);

  printf("Speed msg:%d  Duty Cycle:%f\n\n", motor_speed, duty_cycle);
}

void motor_logic(dbc_DRIVER_STEER_SPEED_s *steer_data) {
  control_motor_steer(steer_data->DRIVER_STEER_direction);
  control_motor_speed(steer_data->DRIVER_STEER_move_speed);
}