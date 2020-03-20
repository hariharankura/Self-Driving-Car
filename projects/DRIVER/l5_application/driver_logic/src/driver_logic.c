#include "driver_logic.h"
#include <stdio.h>
#include <string.h>

static dbc_SENSOR_USONARS_s ultrasonic_sensor_data;
static int8_t DRIVER_STEER_move_FORWARD_at_SPEED = 5;
static int8_t DRIVER_STEER_move_REVERSE_at_SPEED = -5;
static int8_t DRIVER_STEER_move_STOP = 0;

const static uint16_t threshold_ultrasonic_sensor = 10;

void driver_logic__process_ultrasonic_sensors_data(const dbc_SENSOR_USONARS_s sensor_data) {
  ultrasonic_sensor_data = sensor_data;
}

dbc_DRIVER_STEER_SPEED_s driver_logic__get_motor_command(void) {
  dbc_DRIVER_STEER_SPEED_s motor_info = {};
  if (ultrasonic_sensor_data.SENSOR_USONARS_front > threshold_ultrasonic_sensor) {
    motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_FORWARD_at_SPEED;
    if ((ultrasonic_sensor_data.SENSOR_USONARS_right < threshold_ultrasonic_sensor) &&
        (ultrasonic_sensor_data.SENSOR_USONARS_left > threshold_ultrasonic_sensor)) {
      motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT;
    } else if ((ultrasonic_sensor_data.SENSOR_USONARS_left < threshold_ultrasonic_sensor) &&
               (ultrasonic_sensor_data.SENSOR_USONARS_right > threshold_ultrasonic_sensor)) {
      motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_RIGHT;
    } else {
      motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
    }
  } else if (ultrasonic_sensor_data.SENSOR_USONARS_back > threshold_ultrasonic_sensor) {
    motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_REVERSE_at_SPEED;
    if ((ultrasonic_sensor_data.SENSOR_USONARS_right < threshold_ultrasonic_sensor) &&
        (ultrasonic_sensor_data.SENSOR_USONARS_left > threshold_ultrasonic_sensor)) {
      motor_info.DRIVER_STEER_direction =
          DRIVER_STEER_direction_SOFT_RIGHT; // turn slight left, reverse so wheel direction to right
    } else if ((ultrasonic_sensor_data.SENSOR_USONARS_left < threshold_ultrasonic_sensor) &&
               (ultrasonic_sensor_data.SENSOR_USONARS_right > threshold_ultrasonic_sensor)) {
      motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_SOFT_LEFT; // turn slight right
    } else {
      motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
    }
  } else {
    motor_info.DRIVER_STEER_move_speed = DRIVER_STEER_move_STOP;
    motor_info.DRIVER_STEER_direction = DRIVER_STEER_direction_STRAIGHT;
  }
  return motor_info;
}
