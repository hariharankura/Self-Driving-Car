#include "unity.h"

#include "driver_logic.c"

static const uint16_t obstacle_near_threshold = 9;
static const uint16_t obstacle_far_threshold = 11;

void setUp() {}
void tearDown() {}

void test_driver_logic__process_ultrasonic_sensors_data(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_back = 100;
  test_sensor_data.SENSOR_USONARS_front = 200;
  test_sensor_data.SENSOR_USONARS_left = 300;
  test_sensor_data.SENSOR_USONARS_right = 250;
  driver_logic__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_MEMORY(&test_sensor_data, &ultrasonic_sensor_data, sizeof(dbc_SENSOR_USONARS_s));
}

void test_driver_logic__get_motor_command_test_move_forward_turn_right(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_left = obstacle_near_threshold;
  driver_logic__process_ultrasonic_sensors_data(test_sensor_data);
  dbc_DRIVER_STEER_SPEED_s motor_info = driver_logic__get_motor_command();
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_logic__get_motor_command_test_move_forward_turn_left(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_right = obstacle_near_threshold;
  driver_logic__process_ultrasonic_sensors_data(test_sensor_data);
  dbc_DRIVER_STEER_SPEED_s motor_info = driver_logic__get_motor_command();
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_logic__get_motor_command_test_move_forward(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  driver_logic__process_ultrasonic_sensors_data(test_sensor_data);
  dbc_DRIVER_STEER_SPEED_s motor_info = driver_logic__get_motor_command();
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_logic__get_motor_command_test_move_forward_2(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_right = obstacle_near_threshold;
  test_sensor_data.SENSOR_USONARS_left = obstacle_near_threshold;
  driver_logic__process_ultrasonic_sensors_data(test_sensor_data);
  dbc_DRIVER_STEER_SPEED_s motor_info = driver_logic__get_motor_command();
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_logic__get_motor_command_test_move_backwards_turn_left(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_right = obstacle_near_threshold;
  test_sensor_data.SENSOR_USONARS_front = obstacle_near_threshold;
  driver_logic__process_ultrasonic_sensors_data(test_sensor_data);
  dbc_DRIVER_STEER_SPEED_s motor_info = driver_logic__get_motor_command();
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_REVERSE_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_logic__get_motor_command_test_move_backwards_turn_right(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_left = obstacle_near_threshold;
  test_sensor_data.SENSOR_USONARS_front = obstacle_near_threshold;
  driver_logic__process_ultrasonic_sensors_data(test_sensor_data);
  dbc_DRIVER_STEER_SPEED_s motor_info = driver_logic__get_motor_command();
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_REVERSE_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_logic__get_motor_command_test_move_backwards(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_front = obstacle_near_threshold;
  driver_logic__process_ultrasonic_sensors_data(test_sensor_data);
  dbc_DRIVER_STEER_SPEED_s motor_info = driver_logic__get_motor_command();
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_REVERSE_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_logic__get_motor_command_test_move_backwards_2(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_front = obstacle_near_threshold;
  test_sensor_data.SENSOR_USONARS_left = obstacle_near_threshold;
  test_sensor_data.SENSOR_USONARS_right = obstacle_near_threshold;
  driver_logic__process_ultrasonic_sensors_data(test_sensor_data);
  dbc_DRIVER_STEER_SPEED_s motor_info = driver_logic__get_motor_command();
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_REVERSE_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_logic__get_motor_command_test_stop(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_front = obstacle_near_threshold;
  test_sensor_data.SENSOR_USONARS_back = obstacle_near_threshold;
  driver_logic__process_ultrasonic_sensors_data(test_sensor_data);
  dbc_DRIVER_STEER_SPEED_s motor_info = driver_logic__get_motor_command();
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_STOP;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}
