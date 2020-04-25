#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockmotor_logic.h"
#include "Mockspeed_sensor.h"

// Include the source we wish to test
#include "motor_self_test.c"

void setUp(void) {}

void tearDown(void) {}

void test_servo_and_dc_motor_tests(void) {
  servo_and_dc_motor_tests(200);
  TEST_ASSERT_EQUAL_INT32(200, initial_callback_count);
  TEST_ASSERT_FALSE(flag_start);

  control_motor_steer_Expect(DRIVER_STEER_direction_HARD_LEFT);
  control_motor_speed_Expect(0);
  servo_and_dc_motor_tests(250);
  TEST_ASSERT_FALSE(flag_start);

  servo_and_dc_motor_tests(295);

  control_motor_steer_Expect(DRIVER_STEER_direction_HARD_RIGHT);
  control_motor_speed_Expect(0);
  servo_and_dc_motor_tests(300);
  TEST_ASSERT_FALSE(flag_start);

  servo_and_dc_motor_tests(305);

  servo_and_dc_motor_tests(345);

  control_motor_steer_Expect(DRIVER_STEER_direction_STRAIGHT);
  control_motor_speed_Expect(0);
  servo_and_dc_motor_tests(350);
  TEST_ASSERT_FALSE(flag_start);

  servo_and_dc_motor_tests(355);

  servo_and_dc_motor_tests(395);

  control_motor_speed_Expect(2);
  servo_and_dc_motor_tests(400);
  TEST_ASSERT_FALSE(flag_start);

  servo_and_dc_motor_tests(405);

  servo_and_dc_motor_tests(450);

  servo_and_dc_motor_tests(495);

  control_motor_speed_Expect(0);
  servo_and_dc_motor_tests(500);
  TEST_ASSERT_FALSE(flag_start);

  control_motor_speed_Expect(-2);
  servo_and_dc_motor_tests(600);
  TEST_ASSERT_FALSE(flag_start);

  control_motor_speed_Expect(-2);
  servo_and_dc_motor_tests(605);
  TEST_ASSERT_FALSE(flag_start);

  control_motor_speed_Expect(-2);
  servo_and_dc_motor_tests(610);
  TEST_ASSERT_FALSE(flag_start);

  control_motor_speed_Expect(-2);
  servo_and_dc_motor_tests(615);
  TEST_ASSERT_FALSE(flag_start);

  control_motor_speed_Expect(0);
  servo_and_dc_motor_tests(740);
  TEST_ASSERT_FALSE(flag_start);

  reset_motor_test_button_status_Expect();
  servo_and_dc_motor_tests(750);
  TEST_ASSERT_TRUE(flag_start);
}