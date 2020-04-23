#include "unity.h"

#include <stdbool.h>

#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockdelay.h"
#include "Mockgpio.h"
#include "Mockpwm1.h"

#include "motor_logic.c"
#include "project.h"

void setUp(void) {
  gpio_s test_led;
  int16_t motor_speed = 0;
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);
  control_motor_speed(motor_speed);
}

void tearDown(void) {}

void test_init_pwm(void) {
  gpio_s pwm1, pwm2;
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_2, 0, pwm1);
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_2, 1, pwm2);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_2, 0, GPIO__FUNCTION_1, pwm1);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_2, 1, GPIO__FUNCTION_1, pwm2);
  pwm1__init_single_edge_Expect(100);
  init_pwm();
}

void test_control_motor_steer(void) {
  gpio_s test_led;

  DRIVER_STEER_direction_e motor_steer = -2;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, TURN_LEFT_90_DEGREES);
  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);
  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);
  control_motor_steer(motor_steer);

  motor_steer = -1;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, TURN_LEFT_20_DEGREES);
  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);
  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);
  control_motor_steer(motor_steer);

  motor_steer = 0;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, STEER_STRAIGHT);
  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);
  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);
  control_motor_steer(motor_steer);

  motor_steer = 1;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, TURN_RIGHT_20_DEGREES);
  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);
  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);
  control_motor_steer(motor_steer);

  motor_steer = 2;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, TURN_RIGHT_90_DEGREES);
  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);
  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);
  control_motor_steer(motor_steer);
}

void test_control_motor_STOP(void) {
  gpio_s test_led;
  int16_t motor_speed = 0;
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT8(0, reverse_counter);
}

void test_control_motor_forward_direction(void) {
  gpio_s test_led;

  int16_t motor_speed = 4;
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 16.3);
  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT8(0, reverse_counter);

  motor_speed = 2;
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15.9);
  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT8(0, reverse_counter);
}

void test_control_motor_reverse_direction(void) {
  gpio_s test_led;
  uint16_t motor_speed = -1;

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 13.7);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT8(1, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 13.7);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT8(2, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 13.7);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT8(3, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 13.7);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT8(4, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT8(5, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT8(6, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 14.17);
  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT8(7, reverse_counter);

  motor_speed = 0;
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT8(0, reverse_counter);
}