#include "unity.h"

#include <stdbool.h>

#include "Mockcan_bus.h"
#include "Mockgpio.h"

#include "motor_logic.c"
#include "project.h"

void setUp(void) {}

void tearDown(void) {}

void test_init_led(void) {
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_1, 18, left_led);
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_1, 24, right_led);
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_1, 26, fwd_led);
  gpio__construct_as_output_ExpectAndReturn(GPIO__PORT_2, 3, rev_led);
  init_led();
}

void test_motor_logic_steer_left_move_forward(void) {
  dbc_DRIVER_STEER_SPEED_s test_steer_data = {};
  test_steer_data.DRIVER_STEER_direction = -1;
  test_steer_data.DRIVER_STEER_move_speed = 5;
  gpio__reset_Expect(left_led);
  gpio__set_Expect(right_led);
  gpio__reset_Expect(fwd_led);
  gpio__set_Expect(rev_led);
  motor_steer_logic(&test_steer_data);
}

void test_motor_logic_steer_right_move_forward(void) {
  dbc_DRIVER_STEER_SPEED_s test_steer_data = {};
  test_steer_data.DRIVER_STEER_direction = 1;
  test_steer_data.DRIVER_STEER_move_speed = 5;
  gpio__set_Expect(left_led);
  gpio__reset_Expect(right_led);
  gpio__reset_Expect(fwd_led);
  gpio__set_Expect(rev_led);
  motor_steer_logic(&test_steer_data);
}

void test_motor_logic_steer_straight_move_forward(void) {
  dbc_DRIVER_STEER_SPEED_s test_steer_data = {};
  test_steer_data.DRIVER_STEER_direction = 0;
  test_steer_data.DRIVER_STEER_move_speed = 5;
  gpio__set_Expect(left_led);
  gpio__set_Expect(right_led);
  gpio__reset_Expect(fwd_led);
  gpio__set_Expect(rev_led);
  motor_steer_logic(&test_steer_data);
}

void test_motor_logic_steer_straight_move_reverse(void) {
  dbc_DRIVER_STEER_SPEED_s test_steer_data = {};
  test_steer_data.DRIVER_STEER_direction = 0;
  test_steer_data.DRIVER_STEER_move_speed = -5;
  gpio__set_Expect(left_led);
  gpio__set_Expect(right_led);
  gpio__set_Expect(fwd_led);
  gpio__reset_Expect(rev_led);
  motor_steer_logic(&test_steer_data);
}

void test_motor_logic_steer_right_move_reverse(void) {
  dbc_DRIVER_STEER_SPEED_s test_steer_data = {};
  test_steer_data.DRIVER_STEER_direction = 1;
  test_steer_data.DRIVER_STEER_move_speed = -5;
  gpio__set_Expect(left_led);
  gpio__reset_Expect(right_led);
  gpio__set_Expect(fwd_led);
  gpio__reset_Expect(rev_led);
  motor_steer_logic(&test_steer_data);
}

void test_motor_logic_steer_left_move_reverse(void) {
  dbc_DRIVER_STEER_SPEED_s test_steer_data = {};
  test_steer_data.DRIVER_STEER_direction = -1;
  test_steer_data.DRIVER_STEER_move_speed = -5;
  gpio__reset_Expect(left_led);
  gpio__set_Expect(right_led);
  gpio__set_Expect(fwd_led);
  gpio__reset_Expect(rev_led);
  motor_steer_logic(&test_steer_data);
}

void test_motor_logic_steer_straight_and_stop(void) {
  dbc_DRIVER_STEER_SPEED_s test_steer_data = {};
  test_steer_data.DRIVER_STEER_direction = 0;
  test_steer_data.DRIVER_STEER_move_speed = 0;
  gpio__set_Expect(left_led);
  gpio__set_Expect(right_led);
  gpio__set_Expect(fwd_led);
  gpio__set_Expect(rev_led);
  motor_steer_logic(&test_steer_data);
}