#include "unity.h"

#include "Mockgpio.h"
#include "Mocksjvalley_lcd.h"

#include "driver_diagnostics.h"
#include "driving_algo.c"
#include "obstacle_avoidance.h"

static const uint16_t THRESHOLD_OBSTACLE_VERY_NEAR = THRESHOLD_VERY_NEAR - 1;
static const uint16_t THRESHOLD_OBSTACLE_NEAR = THRESHOLD_NEAR - 1;
static const uint16_t THRESHOLD_OBSTACLE_VERY_FAR = THRESHOLD_NEAR + 1;

static dbc_SENSOR_USONARS_s test_sonar_data;

void setUp() {
  test_sonar_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_FAR;
  test_sonar_data.SENSOR_USONARS_back = THRESHOLD_OBSTACLE_VERY_FAR;
  test_sonar_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_VERY_FAR;
  test_sonar_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_VERY_FAR;
}

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
  driving_algo__get_gps_heading_direction(&test_driving_direction);
  TEST_ASSERT_EQUAL(DRIVER_STEER_move_FORWARD_at_SPEED, test_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_HARD_RIGHT, test_driving_direction.DRIVER_STEER_direction);
}

void test_driving_algo__get_gps_heading_direction_soft_right(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 100;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 40;
  dbc_DRIVER_STEER_SPEED_s test_driving_direction = {};
  driving_algo__get_gps_heading_direction(&test_driving_direction);
  TEST_ASSERT_EQUAL(DRIVER_STEER_move_FORWARD_at_SPEED, test_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_SOFT_RIGHT, test_driving_direction.DRIVER_STEER_direction);
}

void test_driving_algo__get_gps_heading_direction_hard_left(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 40;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 140;
  dbc_DRIVER_STEER_SPEED_s test_driving_direction = {};
  driving_algo__get_gps_heading_direction(&test_driving_direction);
  TEST_ASSERT_EQUAL(DRIVER_STEER_move_FORWARD_at_SPEED, test_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_HARD_LEFT, test_driving_direction.DRIVER_STEER_direction);
}

void test_driving_algo__get_gps_heading_direction_soft_left(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 60;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 140;
  dbc_DRIVER_STEER_SPEED_s test_driving_direction = {};
  driving_algo__get_gps_heading_direction(&test_driving_direction);
  TEST_ASSERT_EQUAL(DRIVER_STEER_move_FORWARD_at_SPEED, test_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_SOFT_LEFT, test_driving_direction.DRIVER_STEER_direction);
}

void test_driving_algo__compute_heading_gps_forward_hard_right(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 40;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 300;

  obstacle_avoidance__process_ultrasonic_sensors_data(test_sonar_data);
  gpio__set_Ignore();
  gpio__reset_Ignore();

  dbc_DRIVER_STEER_SPEED_s test_driving_direction = driving_algo__compute_heading();
  TEST_ASSERT_EQUAL(DRIVER_STEER_move_FORWARD_at_SPEED, test_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_HARD_RIGHT, test_driving_direction.DRIVER_STEER_direction);
}

void test_driving_algo__compute_heading_gps_forward_soft_right(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 100;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 40;

  obstacle_avoidance__process_ultrasonic_sensors_data(test_sonar_data);
  gpio__set_Ignore();
  gpio__reset_Ignore();

  dbc_DRIVER_STEER_SPEED_s test_driving_direction = driving_algo__compute_heading();
  TEST_ASSERT_EQUAL(DRIVER_STEER_move_FORWARD_at_SPEED, test_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_SOFT_RIGHT, test_driving_direction.DRIVER_STEER_direction);
}

void test_driving_algo__compute_heading_gps_forward_hard_left(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 40;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 140;

  obstacle_avoidance__process_ultrasonic_sensors_data(test_sonar_data);
  gpio__set_Ignore();
  gpio__reset_Ignore();

  dbc_DRIVER_STEER_SPEED_s test_driving_direction = driving_algo__compute_heading();
  TEST_ASSERT_EQUAL(DRIVER_STEER_move_FORWARD_at_SPEED, test_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_HARD_LEFT, test_driving_direction.DRIVER_STEER_direction);
}

void test_driving_algo__compute_heading_gps_forward_soft_left(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 60;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 140;

  obstacle_avoidance__process_ultrasonic_sensors_data(test_sonar_data);
  gpio__set_Ignore();
  gpio__reset_Ignore();

  dbc_DRIVER_STEER_SPEED_s test_driving_direction = driving_algo__compute_heading();
  TEST_ASSERT_EQUAL(DRIVER_STEER_move_FORWARD_at_SPEED, test_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_SOFT_LEFT, test_driving_direction.DRIVER_STEER_direction);
}

void test_driving_algo__compute_heading_sensor_forward_right(void) {
  current_and_destination_heading_angle.GEO_COMPASS_current_heading = 60;
  current_and_destination_heading_angle.GEO_COMPASS_desitination_heading = 140;

  test_sonar_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sonar_data);
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sonar_data);
  gpio__set_Ignore();
  gpio__reset_Ignore();

  dbc_DRIVER_STEER_SPEED_s test_driving_direction = driving_algo__compute_heading();
  TEST_ASSERT_EQUAL(DRIVER_STEER_move_FORWARD_at_SPEED, test_driving_direction.DRIVER_STEER_move_speed);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_SOFT_RIGHT, test_driving_direction.DRIVER_STEER_direction);
}
