#include "obstacle_avoidance.h"
#include "project_debug.h"

typedef union {
  struct obs {
    uint8_t front : 2;
    uint8_t back : 2;
    uint8_t right : 2;
    uint8_t left : 2;
  } obs;
  uint8_t obstacle_var;
} obstacle_s;

static const uint8_t FRONT_BACK_VERY_NEAR_OBSTACLE = 0x0F;
static const uint8_t FRONT_BACK_NEAR_OBSTACLE = 0x05;
static const uint8_t FRONT_VERY_NEAR_OBSTACLE = 0x03;
static const uint8_t FRONT_NEAR_OBSTACLE = 0x01;
static const uint8_t BACK_VERY_NEAR_OBSTACLE = 0x0C;
static const uint8_t BACK_NEAR_OBSTACLE = 0x04;
static const uint8_t FRONT_BACK_NO_OBSTACLE = 0x00;

static const uint8_t RIGHT_LEFT_VERY_NEAR_OBSTACLE = 0xF0;
static const uint8_t RIGHT_LEFT_NEAR_OBSTACLE = 0x50;
static const uint8_t RIGHT_VERY_NEAR_OBSTACLE = 0x30;
static const uint8_t RIGHT_NEAR_OBSTACLE = 0x10;
static const uint8_t LEFT_VERY_NEAR_OBSTACLE = 0xC0;
static const uint8_t LEFT_NEAR_OBSTACLE = 0x40;
static const uint8_t RIGHT_LEFT_NO_OBSTACLE = 0x00;

static const uint8_t FRONT_NEAR_BACK_VERY_NEAR_OBSTACLE = 0b00001101;
static const uint8_t FRONT_VERY_NEAR_BACK_NEAR_OBSTACLE = 0b00000111;

static dbc_SENSOR_USONARS_s sensor_data;
static obstacle_s ultrasonic_data;
static bool is_obstacle = false;

static void obstacle_avoidance__is_fill_sensor_data() {
  ultrasonic_data.obstacle_var = 0;
  is_obstacle = false;
  if (sensor_data.SENSOR_USONARS_front <= FRONT_BACK_THRESHOLD_VERY_NEAR) {
    ultrasonic_data.obs.front = 3;
    is_obstacle = true;
  } else if (sensor_data.SENSOR_USONARS_front <= FRONT_BACK_THRESHOLD_NEAR) {
    ultrasonic_data.obs.front = 1;
    is_obstacle = true;
  }
  if (sensor_data.SENSOR_USONARS_back <= FRONT_BACK_THRESHOLD_VERY_NEAR) {
    ultrasonic_data.obs.back = 3;
    is_obstacle = true;
  } else if (sensor_data.SENSOR_USONARS_back <= FRONT_BACK_THRESHOLD_NEAR) {
    ultrasonic_data.obs.back = 1;
    is_obstacle = true;
  }
  if (sensor_data.SENSOR_USONARS_right <= THRESHOLD_VERY_NEAR) {
    ultrasonic_data.obs.right = 3;
    is_obstacle = true;
  } else if (sensor_data.SENSOR_USONARS_right <= THRESHOLD_NEAR) {
    ultrasonic_data.obs.right = 1;
    is_obstacle = true;
  }
  if (sensor_data.SENSOR_USONARS_left <= THRESHOLD_VERY_NEAR) {
    ultrasonic_data.obs.left = 3;
    is_obstacle = true;
  } else if (sensor_data.SENSOR_USONARS_left <= THRESHOLD_NEAR) {
    ultrasonic_data.obs.left = 1;
    is_obstacle = true;
  }
}

static void obstacle_avoidance__get_motor_direction(dbc_DRIVER_STEER_SPEED_s *motor_info) {
  switch (ultrasonic_data.obstacle_var & 0x0F) {
  case FRONT_VERY_NEAR_OBSTACLE:
  case FRONT_VERY_NEAR_BACK_NEAR_OBSTACLE:
    motor_info->DRIVER_STEER_move_speed = DRIVER_STEER_move_REVERSE_at_SPEED;
    break;
  case FRONT_NEAR_OBSTACLE:
  case FRONT_BACK_NEAR_OBSTACLE:
  case FRONT_NEAR_BACK_VERY_NEAR_OBSTACLE:
    motor_info->DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_LOW_SPEED;
    break;
  case FRONT_BACK_VERY_NEAR_OBSTACLE:
    motor_info->DRIVER_STEER_move_speed = DRIVER_STEER_move_STOP;
    break;
  case BACK_VERY_NEAR_OBSTACLE:
  case BACK_NEAR_OBSTACLE:
  case FRONT_BACK_NO_OBSTACLE:
  default:
    motor_info->DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
    break;
  }
}

static void obstacle_avoidance__get_steer_direction(dbc_DRIVER_STEER_SPEED_s *motor_info) {
  switch (ultrasonic_data.obstacle_var & 0xF0) {
  case RIGHT_VERY_NEAR_OBSTACLE:
    (motor_info->DRIVER_STEER_move_speed == DRIVER_STEER_move_FORWARD_at_SPEED)
        ? (motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_HARD_LEFT)
        : (motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_HARD_RIGHT);
    break;
  case RIGHT_NEAR_OBSTACLE:
    (motor_info->DRIVER_STEER_move_speed == DRIVER_STEER_move_FORWARD_at_SPEED)
        ? (motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT)
        : (motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT);
    break;
  case LEFT_VERY_NEAR_OBSTACLE:
    (motor_info->DRIVER_STEER_move_speed == DRIVER_STEER_move_FORWARD_at_SPEED)
        ? (motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_HARD_RIGHT)
        : (motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_HARD_LEFT);
    break;
  case LEFT_NEAR_OBSTACLE:
    (motor_info->DRIVER_STEER_move_speed == DRIVER_STEER_move_FORWARD_at_SPEED)
        ? (motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT)
        : (motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT);
    break;
  case RIGHT_LEFT_NO_OBSTACLE:
    (motor_info->DRIVER_STEER_move_speed == DRIVER_STEER_move_REVERSE_at_SPEED)
        ? (motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT)
        : (motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT);
    break;
  case RIGHT_LEFT_VERY_NEAR_OBSTACLE:
  case RIGHT_LEFT_NEAR_OBSTACLE:
  default:
    motor_info->DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
    break;
  }
}
void obstacle_avoidance__process_ultrasonic_sensors_data(const dbc_SENSOR_USONARS_s l_sensor_data) {
  sensor_data = l_sensor_data;
  obstacle_avoidance__is_fill_sensor_data();
}

void obstacle_avoidance__print_debug_data(void) {
  // debug data
  PROJECT_DEBUG__LCD_PRINTF(1, "Obs=%x", ultrasonic_data.obstacle_var);
  PROJECT_DEBUG__PRINTF("Obstacle_Data = %x", ultrasonic_data.obstacle_var);
}

bool obstacle_avoidance__is_required() { return is_obstacle; }

void obstacle_avoidance__get_direction(dbc_DRIVER_STEER_SPEED_s *motor_info) {
  obstacle_avoidance__get_motor_direction(motor_info);
  obstacle_avoidance__get_steer_direction(motor_info);
}
