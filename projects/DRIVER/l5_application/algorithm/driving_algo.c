#include "driving_algo.h"
#include "driver_diagnostics.h"
#include "driver_logic.h"

static int8_t OFFSET_DIRECTION_POSITIVE = 10;
static int8_t OFFSET_DIRECTION_NEGATIVE = -10;

static float driving_algo__compute_deflection(const dbc_GEO_COMPASS_s *heading_angle) {
  float deflection = heading_angle->GEO_COMPASS_current_heading - heading_angle->GEO_COMPASS_desitination_heading;
  if (deflection > 180) {
    deflection -= 360;
  } else if (deflection < -180) {
    deflection += 360;
  }
  return deflection;
}

static void driving_algo__get_gps_heading_direction(dbc_DRIVER_STEER_SPEED_s *driving_direction) {
  dbc_GEO_COMPASS_s heading_angle = driver_logic__get_geo_compass_data();
  float deflection = driving_algo__compute_deflection(&heading_angle);
  if (deflection > OFFSET_DIRECTION_POSITIVE) {
    driving_direction->DRIVER_STEER_direction =
        (deflection < 90) ? DRIVER_STEER_direction_SOFT_RIGHT : DRIVER_STEER_direction_HARD_RIGHT;
  } else if (deflection < OFFSET_DIRECTION_NEGATIVE) {
    driving_direction->DRIVER_STEER_direction =
        (deflection > -90) ? DRIVER_STEER_direction_SOFT_LEFT : DRIVER_STEER_direction_HARD_LEFT;
  } else {
    driving_direction->DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  }
}

static bool driving_algo__is_right_obstacle() {
  bool obstacle_flag = false;
  dbc_SENSOR_USONARS_s ultrasonic_sensor_data = driver_logic__get_ultrasonic_sensors_data();
  if (ultrasonic_sensor_data.SENSOR_USONARS_right < threshold_ultrasonic_sensor) {
    obstacle_flag = true;
  }
  return obstacle_flag;
}

static bool driving_algo__is_left_obstacle() {
  bool obstacle_flag = false;
  dbc_SENSOR_USONARS_s ultrasonic_sensor_data = driver_logic__get_ultrasonic_sensors_data();
  if (ultrasonic_sensor_data.SENSOR_USONARS_left < threshold_ultrasonic_sensor) {
    obstacle_flag = true;
  }
  return obstacle_flag;
}

static bool driving_algo__is_front_obstacle() {
  bool obstacle_flag = false;
  dbc_SENSOR_USONARS_s ultrasonic_sensor_data = driver_logic__get_ultrasonic_sensors_data();
  if (ultrasonic_sensor_data.SENSOR_USONARS_front < threshold_ultrasonic_sensor) {
    obstacle_flag = true;
  }
  return obstacle_flag;
}

static bool driving_algo__is_back_obstacle() {
  bool obstacle_flag = false;
  dbc_SENSOR_USONARS_s ultrasonic_sensor_data = driver_logic__get_ultrasonic_sensors_data();
  if (ultrasonic_sensor_data.SENSOR_USONARS_back < threshold_ultrasonic_sensor) {
    obstacle_flag = true;
  }
  return obstacle_flag;
}

static bool driving_algo__check_if_obstacle_in_direction_algo(dbc_DRIVER_STEER_SPEED_s *driving_direction) {
  bool obstacle_flag = false;
  switch (driving_direction->DRIVER_STEER_direction) {
  case DRIVER_STEER_direction_HARD_LEFT:
  case DRIVER_STEER_direction_SOFT_LEFT:
    obstacle_flag = driving_algo__is_left_obstacle();
    obstacle_flag |= driving_algo__is_front_obstacle();
    break;
  case DRIVER_STEER_direction_STRAIGHT:
    obstacle_flag = driving_algo__is_front_obstacle();
    break;
  case DRIVER_STEER_direction_SOFT_RIGHT:
  case DRIVER_STEER_direction_HARD_RIGHT:
    obstacle_flag = driving_algo__is_right_obstacle();
    obstacle_flag |= driving_algo__is_front_obstacle();
    break;
  default:
    break;
  }
  return obstacle_flag;
}

// Rewrite later
static void driver_algo__get_sensor_direction(dbc_DRIVER_STEER_SPEED_s *motor_info) {
  dbc_SENSOR_USONARS_s ultrasonic_sensor_data = driver_logic__get_ultrasonic_sensors_data();
  if (ultrasonic_sensor_data.SENSOR_USONARS_front > threshold_ultrasonic_sensor) {
    motor_info->DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
    if ((ultrasonic_sensor_data.SENSOR_USONARS_right < threshold_ultrasonic_sensor) &&
        (ultrasonic_sensor_data.SENSOR_USONARS_left > threshold_ultrasonic_sensor)) {
      motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT;
    } else if ((ultrasonic_sensor_data.SENSOR_USONARS_left < threshold_ultrasonic_sensor) &&
               (ultrasonic_sensor_data.SENSOR_USONARS_right > threshold_ultrasonic_sensor)) {
      motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT;
    } else {
      motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
    }
  } else if (ultrasonic_sensor_data.SENSOR_USONARS_back > threshold_ultrasonic_sensor) {
    motor_info->DRIVER_STEER_move_speed = DRIVER_STEER_move_REVERSE_at_SPEED;
    if ((ultrasonic_sensor_data.SENSOR_USONARS_right < threshold_ultrasonic_sensor) &&
        (ultrasonic_sensor_data.SENSOR_USONARS_left > threshold_ultrasonic_sensor)) {
      motor_info->DRIVER_STEER_direction =
          DRIVER_STEER_direction_SOFT_RIGHT; // turn slight left, reverse so wheel direction to right
    } else if ((ultrasonic_sensor_data.SENSOR_USONARS_left < threshold_ultrasonic_sensor) &&
               (ultrasonic_sensor_data.SENSOR_USONARS_right > threshold_ultrasonic_sensor)) {
      motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT; // turn slight right
    } else {
      motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
    }
  } else {
    motor_info->DRIVER_STEER_move_speed = DRIVER_STEER_move_STOP;
    motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  }
  return;
}

dbc_DRIVER_STEER_SPEED_s driving_algo__compute_heading() {
  dbc_DRIVER_STEER_SPEED_s driving_direction;
  driving_algo__get_gps_heading_direction(&driving_direction);
  if (!driving_algo__check_if_obstacle_in_direction_algo(&driving_direction)) {
    // gpio__set(board_io__get_led1());
    driving_direction.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
  } else {
    // gpio__reset(board_io__get_led1());
    driver_algo__get_sensor_direction(&driving_direction); // follow ultrasonic direction
  }
  light_up_direction_led(driving_direction);
  return driving_direction;
}