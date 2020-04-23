#include "motor_self_test.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "motor_logic.h"
#include "speed_sensor.h"

static bool flag_start = true;
static uint32_t initial_callback_count;

void servo_and_dc_motor_tests(uint32_t callback_count) {
  if (flag_start) {
    printf("callback_count: %ld\n", callback_count);
    initial_callback_count = callback_count;
    flag_start = false;
  }
  if (callback_count == initial_callback_count + 50) {
    control_motor_steer(DRIVER_STEER_direction_HARD_LEFT);
    control_motor_speed(0);
  }
  if (callback_count == initial_callback_count + 100) {
    control_motor_steer(DRIVER_STEER_direction_HARD_RIGHT);
    control_motor_speed(0);
  }
  if (callback_count == initial_callback_count + 150) {
    control_motor_steer(DRIVER_STEER_direction_STRAIGHT);
    control_motor_speed(0);
  }
  if (callback_count == initial_callback_count + 200) {
    control_motor_speed(2);
  }
  if (callback_count == initial_callback_count + 300) {
    control_motor_speed(0);
  }
  if (callback_count == initial_callback_count + 400) {
    control_motor_speed(-2);
  }
  if (callback_count == initial_callback_count + 405) {
    control_motor_speed(-2);
  }
  if (callback_count == initial_callback_count + 410) {
    control_motor_speed(-2);
  }
  if (callback_count == initial_callback_count + 415) {
    control_motor_speed(-2);
  }
  if (callback_count == initial_callback_count + 420) {
    control_motor_speed(-2);
  }
  if (callback_count == initial_callback_count + 425) {
    control_motor_speed(-2);
  }
  if (callback_count == initial_callback_count + 430) {
    control_motor_speed(-2);
  }
  if (callback_count == initial_callback_count + 540) {
    control_motor_speed(0);
  }
  if (callback_count == initial_callback_count + 550) {
    flag_start = true;
    reset_motor_test_button_status();
  }
}