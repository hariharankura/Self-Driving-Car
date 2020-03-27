#include "unity.h"

#include "Mockgpio.h"

#include "driver_diagnostics.h"
#include "driver_logic.c"
#include "driving_algo.c"
#include "project.h"

static const uint16_t obstacle_near_threshold = 29;
static const uint16_t obstacle_far_threshold = 31;

void test_driving_algo__compute_deflection_1(void) {
  dbc_GEO_COMPASS_s test_heading_angle;
  test_heading_angle.GEO_COMPASS_current_heading = 340;
  test_heading_angle.GEO_COMPASS_desitination_heading = 260;
  float test_deflection = driving_algo__compute_deflection(&test_heading_angle);
  TEST_ASSERT_EQUAL_FLOAT(80, test_deflection);
}

void test_driving_algo__compute_deflection_2(void) {
  dbc_GEO_COMPASS_s test_heading_angle;
  test_heading_angle.GEO_COMPASS_current_heading = 60;
  test_heading_angle.GEO_COMPASS_desitination_heading = 140;
  float test_deflection = driving_algo__compute_deflection(&test_heading_angle);
  TEST_ASSERT_EQUAL_FLOAT(-80, test_deflection);
}

void test_driving_algo__compute_deflection_3(void) {
  dbc_GEO_COMPASS_s test_heading_angle;
  test_heading_angle.GEO_COMPASS_current_heading = 260;
  test_heading_angle.GEO_COMPASS_desitination_heading = 360;
  float test_deflection = driving_algo__compute_deflection(&test_heading_angle);
  TEST_ASSERT_EQUAL_FLOAT(-100, test_deflection);
}

void test_driving_algo__compute_deflection_4(void) {
  dbc_GEO_COMPASS_s test_heading_angle;
  test_heading_angle.GEO_COMPASS_current_heading = 40;
  test_heading_angle.GEO_COMPASS_desitination_heading = 300;
  float test_deflection = driving_algo__compute_deflection(&test_heading_angle);
  TEST_ASSERT_EQUAL_FLOAT(100, test_deflection);
}

void test_driving_algo__get_gps_heading_direction_hard_right(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 40;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 300;
  dbc_DRIVER_STEER_SPEED_s test_driving_direction = {};
  dbc_DRIVER_STEER_SPEED_s verify_driving_direction = {};
  verify_driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_HARD_RIGHT;
  driving_algo__get_gps_heading_direction(&test_driving_direction);
  TEST_ASSERT_EQUAL_MEMORY(&verify_driving_direction, &test_driving_direction, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driving_algo__get_gps_heading_direction_soft_right(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 100;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 40;
  dbc_DRIVER_STEER_SPEED_s test_driving_direction = {};
  dbc_DRIVER_STEER_SPEED_s verify_driving_direction = {};
  verify_driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT;
  driving_algo__get_gps_heading_direction(&test_driving_direction);
  TEST_ASSERT_EQUAL_MEMORY(&verify_driving_direction, &test_driving_direction, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driving_algo__get_gps_heading_direction_hard_left(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 40;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 140;
  dbc_DRIVER_STEER_SPEED_s test_driving_direction = {};
  dbc_DRIVER_STEER_SPEED_s verify_driving_direction = {};
  verify_driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_HARD_LEFT;
  driving_algo__get_gps_heading_direction(&test_driving_direction);
  TEST_ASSERT_EQUAL_MEMORY(&verify_driving_direction, &test_driving_direction, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driving_algo__get_gps_heading_direction_soft_left(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 60;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 140;
  dbc_DRIVER_STEER_SPEED_s test_driving_direction = {};
  dbc_DRIVER_STEER_SPEED_s verify_driving_direction = {};
  verify_driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT;
  driving_algo__get_gps_heading_direction(&test_driving_direction);
  TEST_ASSERT_EQUAL_MEMORY(&verify_driving_direction, &test_driving_direction, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driving_algo__is_right_obstacle(void) {
  dbc_SENSOR_USONARS_s test_sonar_data = {};
  test_sonar_data.SENSOR_USONARS_right = threshold_ultrasonic_sensor - 1;
  ultrasonic_sensor_data = test_sonar_data;
  TEST_ASSERT_TRUE(driving_algo__is_right_obstacle());
}

void test_driving_algo__is_left_obstacle(void) {
  dbc_SENSOR_USONARS_s test_sonar_data = {};
  test_sonar_data.SENSOR_USONARS_left = threshold_ultrasonic_sensor - 1;
  ultrasonic_sensor_data = test_sonar_data;
  TEST_ASSERT_TRUE(driving_algo__is_left_obstacle());
}

void test_driving_algo__is_front_obstacle(void) {
  dbc_SENSOR_USONARS_s test_sonar_data = {};
  test_sonar_data.SENSOR_USONARS_front = threshold_ultrasonic_sensor - 1;
  ultrasonic_sensor_data = test_sonar_data;
  TEST_ASSERT_TRUE(driving_algo__is_front_obstacle());
}

void test_driving_algo__is_back_obstacle(void) {
  dbc_SENSOR_USONARS_s test_sonar_data = {};
  test_sonar_data.SENSOR_USONARS_back = threshold_ultrasonic_sensor - 1;
  ultrasonic_sensor_data = test_sonar_data;
  TEST_ASSERT_TRUE(driving_algo__is_back_obstacle());
}

void test_driving_algo__check_if_obstacle_in_direction_algo_left(void) {
  dbc_SENSOR_USONARS_s test_sonar_data = {};
  test_sonar_data.SENSOR_USONARS_left = threshold_ultrasonic_sensor - 1;
  ultrasonic_sensor_data = test_sonar_data;
  dbc_DRIVER_STEER_SPEED_s driving_direction = {};
  driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT;
  TEST_ASSERT_TRUE(driving_algo__check_if_obstacle_in_direction_algo(&driving_direction));
}

void test_driving_algo__check_if_obstacle_in_direction_algo_left_front(void) {
  dbc_SENSOR_USONARS_s test_sonar_data = {};
  test_sonar_data.SENSOR_USONARS_front = threshold_ultrasonic_sensor - 1;
  ultrasonic_sensor_data = test_sonar_data;
  dbc_DRIVER_STEER_SPEED_s driving_direction = {};
  driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT;
  TEST_ASSERT_TRUE(driving_algo__check_if_obstacle_in_direction_algo(&driving_direction));
}

void test_driving_algo__check_if_obstacle_in_direction_algo_right(void) {
  dbc_SENSOR_USONARS_s test_sonar_data = {};
  test_sonar_data.SENSOR_USONARS_right = threshold_ultrasonic_sensor - 1;
  ultrasonic_sensor_data = test_sonar_data;
  dbc_DRIVER_STEER_SPEED_s driving_direction = {};
  driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT;
  TEST_ASSERT_TRUE(driving_algo__check_if_obstacle_in_direction_algo(&driving_direction));
}

void test_driving_algo__check_if_obstacle_in_direction_algo_right_front(void) {
  dbc_SENSOR_USONARS_s test_sonar_data = {};
  test_sonar_data.SENSOR_USONARS_front = threshold_ultrasonic_sensor - 1;
  ultrasonic_sensor_data = test_sonar_data;
  dbc_DRIVER_STEER_SPEED_s driving_direction = {};
  driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT;
  TEST_ASSERT_TRUE(driving_algo__check_if_obstacle_in_direction_algo(&driving_direction));
}

void test_driving_algo__check_if_obstacle_in_direction_algo_front(void) {
  dbc_SENSOR_USONARS_s test_sonar_data = {};
  test_sonar_data.SENSOR_USONARS_front = threshold_ultrasonic_sensor - 1;
  ultrasonic_sensor_data = test_sonar_data;
  dbc_DRIVER_STEER_SPEED_s driving_direction = {};
  driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  TEST_ASSERT_TRUE(driving_algo__check_if_obstacle_in_direction_algo(&driving_direction));
}

void test_driving_algo__check_if_obstacle_in_direction_algo_none(void) {
  dbc_SENSOR_USONARS_s test_sonar_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                          .SENSOR_USONARS_right = obstacle_far_threshold,
                                          .SENSOR_USONARS_front = obstacle_far_threshold,
                                          .SENSOR_USONARS_back = obstacle_far_threshold};
  ultrasonic_sensor_data = test_sonar_data;
  dbc_DRIVER_STEER_SPEED_s driving_direction = {};
  driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  TEST_ASSERT_FALSE(driving_algo__check_if_obstacle_in_direction_algo(&driving_direction));
}

void test_driver_algo__get_sensor_direction_test_move_forward_turn_right(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_left = obstacle_near_threshold;
  ultrasonic_sensor_data = test_sensor_data;
  dbc_DRIVER_STEER_SPEED_s motor_info;
  driver_algo__get_sensor_direction(&motor_info);
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_algo__get_sensor_direction_test_move_forward_turn_left(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_right = obstacle_near_threshold;
  ultrasonic_sensor_data = test_sensor_data;
  dbc_DRIVER_STEER_SPEED_s motor_info;

  driver_algo__get_sensor_direction(&motor_info);
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_algo__get_sensor_direction_test_move_forward(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  ultrasonic_sensor_data = test_sensor_data;
  dbc_DRIVER_STEER_SPEED_s motor_info;
  driver_algo__get_sensor_direction(&motor_info);
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_algo__get_sensor_direction_test_move_forward_2(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_right = obstacle_near_threshold;
  test_sensor_data.SENSOR_USONARS_left = obstacle_near_threshold;
  ultrasonic_sensor_data = test_sensor_data;
  dbc_DRIVER_STEER_SPEED_s motor_info;
  driver_algo__get_sensor_direction(&motor_info);
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_algo__get_sensor_direction_test_move_backwards_turn_left(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_right = obstacle_near_threshold;
  test_sensor_data.SENSOR_USONARS_front = obstacle_near_threshold;
  ultrasonic_sensor_data = test_sensor_data;
  dbc_DRIVER_STEER_SPEED_s motor_info;
  driver_algo__get_sensor_direction(&motor_info);
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_REVERSE_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_algo__get_sensor_direction_test_move_backwards_turn_right(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_left = obstacle_near_threshold;
  test_sensor_data.SENSOR_USONARS_front = obstacle_near_threshold;
  ultrasonic_sensor_data = test_sensor_data;
  dbc_DRIVER_STEER_SPEED_s motor_info;
  driver_algo__get_sensor_direction(&motor_info);
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_REVERSE_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_algo__get_sensor_direction_test_move_backwards(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_front = obstacle_near_threshold;
  ultrasonic_sensor_data = test_sensor_data;
  dbc_DRIVER_STEER_SPEED_s motor_info;
  driver_algo__get_sensor_direction(&motor_info);
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_REVERSE_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_algo__get_sensor_direction_test_move_backwards_2(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_front = obstacle_near_threshold;
  test_sensor_data.SENSOR_USONARS_left = obstacle_near_threshold;
  test_sensor_data.SENSOR_USONARS_right = obstacle_near_threshold;
  ultrasonic_sensor_data = test_sensor_data;
  dbc_DRIVER_STEER_SPEED_s motor_info;
  driver_algo__get_sensor_direction(&motor_info);
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_REVERSE_at_SPEED;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driver_algo__get_sensor_direction_test_stop(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_front = obstacle_near_threshold;
  test_sensor_data.SENSOR_USONARS_back = obstacle_near_threshold;
  ultrasonic_sensor_data = test_sensor_data;
  dbc_DRIVER_STEER_SPEED_s motor_info;
  driver_algo__get_sensor_direction(&motor_info);
  dbc_DRIVER_STEER_SPEED_s verify_motor_info = {};
  verify_motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  verify_motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_STOP;
  TEST_ASSERT_EQUAL_MEMORY(&motor_info, &verify_motor_info, sizeof(dbc_DRIVER_STEER_SPEED_s));
}

void test_driving_algo__compute_heading_gps_forward_hard_right(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 40;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 300;
  dbc_DRIVER_STEER_SPEED_s verify_driving_direction = {};
  verify_driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_HARD_RIGHT;
  verify_driving_direction.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;

  dbc_SENSOR_USONARS_s test_sonar_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                          .SENSOR_USONARS_right = obstacle_far_threshold,
                                          .SENSOR_USONARS_front = obstacle_far_threshold,
                                          .SENSOR_USONARS_back = obstacle_far_threshold};
  ultrasonic_sensor_data = test_sonar_data;
  gpio__set_Ignore();
  gpio__reset_Ignore();

  dbc_DRIVER_STEER_SPEED_s test_driving_direction = driving_algo__compute_heading();
  TEST_ASSERT_EQUAL(test_driving_direction.DRIVER_STEER_move_speed, verify_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(test_driving_direction.DRIVER_STEER_direction, verify_driving_direction.DRIVER_STEER_direction);
}

void test_driving_algo__compute_heading_gps_forward_soft_right(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 100;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 40;
  dbc_DRIVER_STEER_SPEED_s verify_driving_direction = {};
  verify_driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT;
  verify_driving_direction.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;

  dbc_SENSOR_USONARS_s test_sonar_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                          .SENSOR_USONARS_right = obstacle_far_threshold,
                                          .SENSOR_USONARS_front = obstacle_far_threshold,
                                          .SENSOR_USONARS_back = obstacle_far_threshold};
  ultrasonic_sensor_data = test_sonar_data;
  gpio__set_Ignore();
  gpio__reset_Ignore();

  dbc_DRIVER_STEER_SPEED_s test_driving_direction = driving_algo__compute_heading();
  TEST_ASSERT_EQUAL(test_driving_direction.DRIVER_STEER_move_speed, verify_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(test_driving_direction.DRIVER_STEER_direction, verify_driving_direction.DRIVER_STEER_direction);
}

void test_driving_algo__compute_heading_gps_forward_hard_left(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 40;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 140;
  dbc_DRIVER_STEER_SPEED_s verify_driving_direction = {};
  verify_driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_HARD_LEFT;
  verify_driving_direction.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;

  dbc_SENSOR_USONARS_s test_sonar_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                          .SENSOR_USONARS_right = obstacle_far_threshold,
                                          .SENSOR_USONARS_front = obstacle_far_threshold,
                                          .SENSOR_USONARS_back = obstacle_far_threshold};
  ultrasonic_sensor_data = test_sonar_data;
  gpio__set_Ignore();
  gpio__reset_Ignore();

  dbc_DRIVER_STEER_SPEED_s test_driving_direction = driving_algo__compute_heading();
  TEST_ASSERT_EQUAL(test_driving_direction.DRIVER_STEER_move_speed, verify_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(test_driving_direction.DRIVER_STEER_direction, verify_driving_direction.DRIVER_STEER_direction);
}

void test_driving_algo__compute_heading_gps_forward_soft_left(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 60;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 140;
  dbc_DRIVER_STEER_SPEED_s verify_driving_direction = {};
  verify_driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT;
  verify_driving_direction.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;

  dbc_SENSOR_USONARS_s test_sonar_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                          .SENSOR_USONARS_right = obstacle_far_threshold,
                                          .SENSOR_USONARS_front = obstacle_far_threshold,
                                          .SENSOR_USONARS_back = obstacle_far_threshold};
  ultrasonic_sensor_data = test_sonar_data;
  gpio__set_Ignore();
  gpio__reset_Ignore();

  dbc_DRIVER_STEER_SPEED_s test_driving_direction = driving_algo__compute_heading();
  TEST_ASSERT_EQUAL(test_driving_direction.DRIVER_STEER_move_speed, verify_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(test_driving_direction.DRIVER_STEER_direction, verify_driving_direction.DRIVER_STEER_direction);
}

void test_driving_algo__compute_heading_sensor_forward_right(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 60;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 140;
  dbc_DRIVER_STEER_SPEED_s verify_driving_direction = {};
  verify_driving_direction.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT;
  verify_driving_direction.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;

  dbc_SENSOR_USONARS_s test_sonar_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                          .SENSOR_USONARS_right = obstacle_far_threshold,
                                          .SENSOR_USONARS_front = obstacle_far_threshold,
                                          .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sonar_data.SENSOR_USONARS_left = obstacle_near_threshold;
  ultrasonic_sensor_data = test_sonar_data;
  gpio__set_Ignore();
  gpio__reset_Ignore();

  dbc_DRIVER_STEER_SPEED_s test_driving_direction = driving_algo__compute_heading();
  TEST_ASSERT_EQUAL(test_driving_direction.DRIVER_STEER_move_speed, verify_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(test_driving_direction.DRIVER_STEER_direction, verify_driving_direction.DRIVER_STEER_direction);
}
