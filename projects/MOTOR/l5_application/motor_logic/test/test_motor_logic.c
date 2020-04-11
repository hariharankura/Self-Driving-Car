#include "unity.h"

#include <stdbool.h>

#include "Mockcan_bus.h"
#include "Mockdelay.h"
#include "Mockgpio.h"
#include "Mockpwm1.h"

#include "motor_logic.c"
#include "project.h"

void setUp(void) {}

void tearDown(void) {}

void test_init_pwm(void) {
  gpio_s pwm1, pwm2;
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_2, 0, pwm1);
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_2, 1, pwm2);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_2, 0, GPIO__FUNCTION_1, pwm1);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_2, 1, GPIO__FUNCTION_1, pwm2);
  pwm1__init_single_edge_Expect(1000);
  init_pwm();
}

void test_control_motor_steer(void) {
  DRIVER_STEER_direction_e motor_steer = -2;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, 10);
  control_motor_steer(motor_steer);

  motor_steer = -1;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, 13);
  control_motor_steer(motor_steer);

  motor_steer = 0;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, 15);
  control_motor_steer(motor_steer);

  motor_steer = 1;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, 17);
  control_motor_steer(motor_steer);

  motor_steer = 2;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, 20);
  control_motor_steer(motor_steer);
}

void test_control_motor_speed(void) {
  int16_t motor_speed = 10;
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  delay__ms_Expect(100);
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 20);
  control_motor_speed(motor_speed);

  motor_speed = 4;
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  delay__ms_Expect(100);
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 17);
  control_motor_speed(motor_speed);

  motor_speed = 0;
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  delay__ms_Expect(100);
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  control_motor_speed(motor_speed);

  motor_speed = -6;
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  delay__ms_Expect(100);
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 12);
  control_motor_speed(motor_speed);

  motor_speed = -10;
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  delay__ms_Expect(100);
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 10);
  control_motor_speed(motor_speed);
}

void test_motor_logic_steer_right_move_forward(void) {
  dbc_DRIVER_STEER_SPEED_s test_steer_data = {};
  test_steer_data.DRIVER_STEER_direction = 1;
  test_steer_data.DRIVER_STEER_move_speed = 5;

  pwm1__set_duty_cycle_Expect(PWM_SERVO, 17);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  delay__ms_Expect(100);
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 17.5);

  motor_logic(&test_steer_data);
}

void test_motor_logic_steer_left_move_reverse(void) {
  dbc_DRIVER_STEER_SPEED_s test_steer_data = {};
  test_steer_data.DRIVER_STEER_direction = -1;
  test_steer_data.DRIVER_STEER_move_speed = -5;

  pwm1__set_duty_cycle_Expect(PWM_SERVO, 13);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  delay__ms_Expect(100);
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 12.5);

  motor_logic(&test_steer_data);
}

void test_motor_logic_steer_straight_and_stop(void) {
  dbc_DRIVER_STEER_SPEED_s test_steer_data = {};
  test_steer_data.DRIVER_STEER_direction = 0;
  test_steer_data.DRIVER_STEER_move_speed = 0;

  pwm1__set_duty_cycle_Expect(PWM_SERVO, 15);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  delay__ms_Expect(100);
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);

  motor_logic(&test_steer_data);
}