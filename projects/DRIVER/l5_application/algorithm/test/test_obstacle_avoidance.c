#include "unity.h"

#include "obstacle_avoidance.c"

dbc_SENSOR_USONARS_s test_sensor_data;
dbc_DRIVER_STEER_SPEED_s test_motor_info;

static const uint16_t THRESHOLD_OBSTACLE_VERY_NEAR = THRESHOLD_VERY_NEAR - 1;
static const uint16_t THRESHOLD_OBSTACLE_NEAR = THRESHOLD_NEAR - 1;
static const uint16_t THRESHOLD_OBSTACLE_VERY_FAR = THRESHOLD_NEAR + 1;

void setUp() {
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_FAR;
  test_sensor_data.SENSOR_USONARS_back = THRESHOLD_OBSTACLE_VERY_FAR;
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_VERY_FAR;
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_VERY_FAR;
}

void tearDown() {}

void test_obstacle_avoidance__process_ultrasonic_sensors_data(void) {
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT16(THRESHOLD_OBSTACLE_VERY_FAR, sensor_data.SENSOR_USONARS_front);
  TEST_ASSERT_EQUAL_UINT16(THRESHOLD_OBSTACLE_VERY_FAR, sensor_data.SENSOR_USONARS_back);
  TEST_ASSERT_EQUAL_UINT16(THRESHOLD_OBSTACLE_VERY_FAR, sensor_data.SENSOR_USONARS_right);
  TEST_ASSERT_EQUAL_UINT16(THRESHOLD_OBSTACLE_VERY_FAR, sensor_data.SENSOR_USONARS_left);
}

void test_obstacle_avoidance__at_front_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(FRONT_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(1, ultrasonic_data.obs.front);
}

void test_obstacle_avoidance__at_front_VERY_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(FRONT_VERY_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.front);
}

void test_obstacle_avoidance__at_back_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_back = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(BACK_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(1, ultrasonic_data.obs.back);
}

void test_obstacle_avoidance__at_back_VERY_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_back = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(BACK_VERY_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.back);
}

void test_obstacle_avoidance__at_front_back_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_NEAR;
  test_sensor_data.SENSOR_USONARS_back = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(FRONT_BACK_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(1, ultrasonic_data.obs.back);
  TEST_ASSERT_EQUAL_UINT8(1, ultrasonic_data.obs.front);
}

void test_obstacle_avoidance__at_front_back_VERY_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_back = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(FRONT_BACK_VERY_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.back);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.front);
}

void test_obstacle_avoidance__at_right_near_is_fill_sensor_data() {
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(RIGHT_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(1, ultrasonic_data.obs.right);
}

void test_obstacle_avoidance__at_right_VERY_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(RIGHT_VERY_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.right);
}

void test_obstacle_avoidance__at_left_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(LEFT_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(1, ultrasonic_data.obs.left);
}

void test_obstacle_avoidance__at_left_VERY_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(LEFT_VERY_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.left);
}

void test_obstacle_avoidance__at_right_left_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_NEAR;
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(RIGHT_LEFT_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(1, ultrasonic_data.obs.right);
  TEST_ASSERT_EQUAL_UINT8(1, ultrasonic_data.obs.left);
}

void test_obstacle_avoidance__at_right_left_VERY_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(RIGHT_LEFT_VERY_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.right);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.left);
}

void test_obstacle_avoidance__at_front_right_VERY_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(RIGHT_VERY_NEAR_OBSTACLE | FRONT_VERY_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.right);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.front);
}

void test_obstacle_avoidance__at_front_left_VERY_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(LEFT_VERY_NEAR_OBSTACLE | FRONT_VERY_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.left);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.front);
}

void test_obstacle_avoidance__at_front_near_right_VERY_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(RIGHT_VERY_NEAR_OBSTACLE | FRONT_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.right);
  TEST_ASSERT_EQUAL_UINT8(1, ultrasonic_data.obs.front);
}

void test_obstacle_avoidance__at_front_near_left_VERY_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(LEFT_VERY_NEAR_OBSTACLE | FRONT_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.left);
  TEST_ASSERT_EQUAL_UINT8(1, ultrasonic_data.obs.front);
}

void test_obstacle_avoidance__at_front_VERY_right_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(RIGHT_NEAR_OBSTACLE | FRONT_VERY_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(1, ultrasonic_data.obs.right);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.front);
}

void test_obstacle_avoidance__at_front_VERY_left_near_is_fill_sensor_data(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(LEFT_NEAR_OBSTACLE | FRONT_VERY_NEAR_OBSTACLE, ultrasonic_data.obstacle_var);
  TEST_ASSERT_EQUAL_UINT8(1, ultrasonic_data.obs.left);
  TEST_ASSERT_EQUAL_UINT8(3, ultrasonic_data.obs.front);
}

void test_obstacle_avoidance__nothing_near_is_fill_sensor_data(void) {
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_UINT8(0, ultrasonic_data.obstacle_var);
}

void test_obstacle_avoidance__in_near_left_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_SOFT_RIGHT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_FORWARD_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_very_near_left_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_HARD_RIGHT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_FORWARD_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
  obstacle_avoidance__get_steer_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_HARD_RIGHT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_FORWARD_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_near_right_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_SOFT_LEFT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_FORWARD_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);

  obstacle_avoidance__get_steer_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_SOFT_LEFT, test_motor_info.DRIVER_STEER_direction);
}

void test_obstacle_avoidance__in_very_near_right_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_HARD_LEFT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_FORWARD_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
  obstacle_avoidance__get_steer_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_HARD_LEFT, test_motor_info.DRIVER_STEER_direction);
}

void test_obstacle_avoidance__in_near_right_left_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_NEAR;
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_STRAIGHT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_FORWARD_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
  obstacle_avoidance__get_steer_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_STRAIGHT, test_motor_info.DRIVER_STEER_direction);
}

void test_obstacle_avoidance__in_very_near_right_left_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_STRAIGHT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_FORWARD_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
  obstacle_avoidance__get_steer_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_STRAIGHT, test_motor_info.DRIVER_STEER_direction);
}

void test_obstacle_avoidance__in_near_front_left_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_SOFT_LEFT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_REVERSE_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_very_near_front_left_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_HARD_LEFT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_REVERSE_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_near_front_right_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_SOFT_RIGHT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_REVERSE_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_very_near_front_right_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_HARD_RIGHT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_REVERSE_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_near_right_left_front_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_NEAR;
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_STRAIGHT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_REVERSE_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_very_near_right_left_front_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_STRAIGHT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_REVERSE_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_very_near_right_left_front_back_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_left = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_right = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_back = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_STRAIGHT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_STOP, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_very_near_front_back_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_NEAR;
  test_sensor_data.SENSOR_USONARS_back = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_STRAIGHT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_STOP, test_motor_info.DRIVER_STEER_move_speed);
  obstacle_avoidance__get_motor_direction(&test_motor_info);
  ;
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_STOP, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_near_front_back_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_NEAR;
  test_sensor_data.SENSOR_USONARS_back = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_direction(&test_motor_info);
  TEST_ASSERT_EQUAL(DRIVER_STEER_direction_STRAIGHT, test_motor_info.DRIVER_STEER_direction);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_STOP, test_motor_info.DRIVER_STEER_move_speed);
  obstacle_avoidance__get_motor_direction(&test_motor_info);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_STOP, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_near_front_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_motor_direction(&test_motor_info);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_REVERSE_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_very_near_front_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_front = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_motor_direction(&test_motor_info);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_REVERSE_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_near_back_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_back = THRESHOLD_OBSTACLE_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_motor_direction(&test_motor_info);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_FORWARD_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
}

void test_obstacle_avoidance__in_very_near_back_get_direction(void) {
  test_sensor_data.SENSOR_USONARS_back = THRESHOLD_OBSTACLE_VERY_NEAR;
  obstacle_avoidance__process_ultrasonic_sensors_data(test_sensor_data);
  obstacle_avoidance__get_motor_direction(&test_motor_info);
  TEST_ASSERT_EQUAL_INT16(DRIVER_STEER_move_FORWARD_at_SPEED, test_motor_info.DRIVER_STEER_move_speed);
}