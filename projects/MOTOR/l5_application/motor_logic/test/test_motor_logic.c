#include "unity.h"

#include <stdbool.h>

#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockdelay.h"
#include "Mockgpio.h"
#include "Mockpwm1.h"
#include "Mockspeed_sensor.h"

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
  pwm1__init_single_edge_Expect(100);
  init_pwm();
}

void test_control_motor_steer(void) {

  DRIVER_STEER_direction_e motor_steer = -2;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, TURN_LEFT_90_DEGREES);
  control_motor_steer(motor_steer);

  motor_steer = -1;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, TURN_LEFT_20_DEGREES);
  control_motor_steer(motor_steer);

  motor_steer = 0;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, STEER_STRAIGHT);
  control_motor_steer(motor_steer);

  motor_steer = 1;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, TURN_RIGHT_20_DEGREES);
  control_motor_steer(motor_steer);

  motor_steer = 2;
  pwm1__set_duty_cycle_Expect(PWM_SERVO, TURN_RIGHT_90_DEGREES);
  control_motor_steer(motor_steer);
}

void test_control_motor_STOP(void) {
  gpio_s test_led;
  int16_t motor_speed = 0;
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);

  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);

  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT32(0, reverse_counter);
  TEST_ASSERT_FALSE(reverse_flag);
  TEST_ASSERT_EQUAL_FLOAT(15.8, pwm_forward);
}

void test_control_motor_forward_direction_current_speed_less_than_target_speed(void) {
  gpio_s test_led;
  int16_t target_speed_mph = 2;
  pwm_forward = 15.9;

  get_mph_ExpectAndReturn(1.8);                   // current_speed_mph = 1.8
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15.902); // pwm_forward increased by 0.002

  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);

  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  control_motor_speed(target_speed_mph);
  TEST_ASSERT_EQUAL_INT32(0, reverse_counter);
  TEST_ASSERT_FALSE(reverse_flag);
  TEST_ASSERT_EQUAL_FLOAT(15.902, pwm_forward);
}

void test_control_motor_forward_direction_current_speed_greater_than_target_speed(void) {
  gpio_s test_led;
  int16_t target_speed_mph = 2;
  pwm_forward = 15.9;

  get_mph_ExpectAndReturn(3);                     // current_speed_mph = 3
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15.896); // pwm_forward decreased by 0.004

  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);

  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  control_motor_speed(target_speed_mph);
  TEST_ASSERT_EQUAL_INT32(0, reverse_counter);
  TEST_ASSERT_FALSE(reverse_flag);
  TEST_ASSERT_EQUAL_FLOAT(15.896, pwm_forward);
}

void test_braking_when_current_speed_is_greater_than_target_speed(void) {
  gpio_s test_led;
  int16_t target_speed_mph = 2;
  pwm_forward = 16.1;

  get_mph_ExpectAndReturn(4); // current_speed_mph = 4, Speed difference is 2
  pwm1__set_duty_cycle_Expect(PWM_MOTOR,
                              10); // pwm value 10 is for braking(brake applied as speed diff is greater than 1.5)

  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);

  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  control_motor_speed(target_speed_mph);
  TEST_ASSERT_EQUAL_INT32(0, reverse_counter);
  TEST_ASSERT_FALSE(reverse_flag);
  TEST_ASSERT_EQUAL_FLOAT(15.8, pwm_forward); // After brake is applied, pwm will start increasing from low_default
                                              // value

  pwm_forward = 16;
  get_mph_ExpectAndReturn(3.4);                   // current_speed_mph = 9, Speed difference is 1.4
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15.996); // No brake is applied as speed diff is not greater than 1.5

  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);

  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  control_motor_speed(target_speed_mph);
  TEST_ASSERT_EQUAL_INT32(0, reverse_counter);
  TEST_ASSERT_FALSE(reverse_flag);
  TEST_ASSERT_EQUAL_FLOAT(15.996, pwm_forward);
}

void test_control_motor_forward_direction_limit_pwm_when_reaches_max(void) {
  gpio_s test_led;
  int16_t target_speed_mph = 1;
  pwm_forward = 18;

  get_mph_ExpectAndReturn(0.5);               // current_speed_mph = 0.5
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 18); // pwm_forward is limited to 18

  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);

  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  control_motor_speed(target_speed_mph);
  TEST_ASSERT_EQUAL_INT32(0, reverse_counter);
  TEST_ASSERT_FALSE(reverse_flag);
  TEST_ASSERT_EQUAL_FLOAT(18, pwm_forward);
}

void test_control_motor_forward_direction_limit_pwm_when_reaches_min(void) {
  gpio_s test_led;
  int16_t target_speed_mph = 1;
  pwm_forward = 15.8;

  get_mph_ExpectAndReturn(2);                   // current_speed_mph = 2
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15.8); // pwm_forward is limited to 15.8

  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);

  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  control_motor_speed(target_speed_mph);
  TEST_ASSERT_EQUAL_INT32(0, reverse_counter);
  TEST_ASSERT_FALSE(reverse_flag);
  TEST_ASSERT_EQUAL_FLOAT(15.8, pwm_forward);
}

void test_control_motor_reverse_direction(void) {
  gpio_s test_led;
  uint16_t motor_speed = -1;

  TEST_ASSERT_EQUAL_INT32(0, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 10);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT32(1, reverse_counter);
  TEST_ASSERT_TRUE(reverse_flag);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 10);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT32(2, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 10);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT32(3, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 10);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT32(4, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT32(5, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT32(6, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 14.07);

  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);

  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT32(7, reverse_counter);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 14.07);

  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);

  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT32(8, reverse_counter);

  for (int i = 0; i < 100; i++) {
    pwm1__set_duty_cycle_Expect(PWM_MOTOR, 14.07);

    board_io__get_led1_ExpectAndReturn(test_led);
    gpio__reset_Expect(test_led);

    board_io__get_led2_ExpectAndReturn(test_led);
    gpio__set_Expect(test_led);

    board_io__get_led3_ExpectAndReturn(test_led);
    gpio__set_Expect(test_led);

    control_motor_speed(motor_speed);
  }

  TEST_ASSERT_EQUAL_INT32(108, reverse_counter);

  motor_speed = 0;
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);

  board_io__get_led1_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  board_io__get_led2_ExpectAndReturn(test_led);
  gpio__reset_Expect(test_led);

  board_io__get_led3_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);

  control_motor_speed(motor_speed);
  TEST_ASSERT_EQUAL_INT32(0, reverse_counter);
  TEST_ASSERT_FALSE(reverse_flag);
  TEST_ASSERT_EQUAL_FLOAT(15.8, pwm_forward);
}

void test_motor_speed_with_direction(void) {
  reverse_flag = false;
  get_mph_ExpectAndReturn(3.2);
  TEST_ASSERT_EQUAL_FLOAT(3.2, motor_speed_with_direction());

  reverse_flag = true;
  get_mph_ExpectAndReturn(3.2);
  TEST_ASSERT_EQUAL_FLOAT(-3.2, motor_speed_with_direction());
}

void test_get_pwm_forward(void) {
  pwm_forward = 15.9;
  TEST_ASSERT_EQUAL_FLOAT(15.9, get_pwm_forward());

  pwm_forward = 18;
  TEST_ASSERT_EQUAL_FLOAT(18, get_pwm_forward());
}