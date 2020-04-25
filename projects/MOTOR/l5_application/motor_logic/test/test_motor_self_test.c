#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockmotor_logic.h"
#include "Mockpwm1.h"

// Include the source we wish to test
#include "motor_self_test.c"

void setUp(void) {}

void tearDown(void) {}

void test_get_motor_test_button_status(void) {
  motor_test_button_status = 1;
  TEST_ASSERT_EQUAL_UINT8(1, get_motor_test_button_status());

  motor_test_button_status = 0;
  TEST_ASSERT_EQUAL_UINT8(0, get_motor_test_button_status());
}

void test_set_motor_test_button_status(void) {
  set_motor_test_button_status(1);
  TEST_ASSERT_EQUAL_UINT8(1, motor_test_button_status);

  set_motor_test_button_status(0);
  TEST_ASSERT_EQUAL_UINT8(0, motor_test_button_status);
}

void test_servo_and_dc_motor_tests(void) {
  motor_test_button_status = 1;

  servo_and_dc_motor_tests(200);
  TEST_ASSERT_EQUAL_INT32(200, initial_callback_count);
  TEST_ASSERT_FALSE(flag_start);
  TEST_ASSERT_EQUAL_UINT8(1, motor_test_button_status);

  pwm1__set_duty_cycle_Expect(PWM_SERVO, TURN_LEFT_90_DEGREES);
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  servo_and_dc_motor_tests(250);
  TEST_ASSERT_FALSE(flag_start);
  TEST_ASSERT_EQUAL_UINT8(1, motor_test_button_status);

  servo_and_dc_motor_tests(295);

  pwm1__set_duty_cycle_Expect(PWM_SERVO, TURN_RIGHT_90_DEGREES);
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  servo_and_dc_motor_tests(300);
  TEST_ASSERT_FALSE(flag_start);
  TEST_ASSERT_EQUAL_UINT8(1, motor_test_button_status);

  servo_and_dc_motor_tests(305);

  servo_and_dc_motor_tests(345);

  pwm1__set_duty_cycle_Expect(PWM_SERVO, STEER_STRAIGHT);
  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  servo_and_dc_motor_tests(350);
  TEST_ASSERT_FALSE(flag_start);
  TEST_ASSERT_EQUAL_UINT8(1, motor_test_button_status);

  servo_and_dc_motor_tests(355);

  servo_and_dc_motor_tests(395);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15.9);
  servo_and_dc_motor_tests(400);
  TEST_ASSERT_FALSE(flag_start);
  TEST_ASSERT_EQUAL_UINT8(1, motor_test_button_status);

  servo_and_dc_motor_tests(405);

  servo_and_dc_motor_tests(450);

  servo_and_dc_motor_tests(495);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  servo_and_dc_motor_tests(500);
  TEST_ASSERT_FALSE(flag_start);
  TEST_ASSERT_EQUAL_UINT8(1, motor_test_button_status);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 14.1);
  servo_and_dc_motor_tests(600);
  TEST_ASSERT_FALSE(flag_start);
  TEST_ASSERT_EQUAL_UINT8(1, motor_test_button_status);

  pwm1__set_duty_cycle_Expect(PWM_MOTOR, 15);
  servo_and_dc_motor_tests(700);
  TEST_ASSERT_FALSE(flag_start);
  TEST_ASSERT_EQUAL_UINT8(1, motor_test_button_status);

  servo_and_dc_motor_tests(750);
  TEST_ASSERT_TRUE(flag_start);
  TEST_ASSERT_EQUAL_UINT8(0, motor_test_button_status);
}