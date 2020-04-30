#include "motor_self_test.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "motor_logic.h"
#include "pwm1.h"

static uint8_t motor_test_button_status = 0;

static bool flag_start = true;
static uint32_t initial_callback_count;

uint8_t get_motor_test_button_status(void) { return motor_test_button_status; }

void set_motor_test_button_status(uint8_t status) { motor_test_button_status = status; }

void servo_and_dc_motor_tests(uint32_t callback_count) {
  if (flag_start) {
    printf("callback_count: %ld\n", callback_count);
    initial_callback_count = callback_count;
    flag_start = false;
  }
  if (callback_count == initial_callback_count + 50) {
    pwm1__set_duty_cycle(PWM_SERVO, TURN_LEFT_90_DEGREES);
    pwm1__set_duty_cycle(PWM_MOTOR, 15);
  }
  if (callback_count == initial_callback_count + 100) {
    pwm1__set_duty_cycle(PWM_SERVO, TURN_RIGHT_90_DEGREES);
    pwm1__set_duty_cycle(PWM_MOTOR, 15);
  }
  if (callback_count == initial_callback_count + 150) {
    pwm1__set_duty_cycle(PWM_SERVO, STEER_STRAIGHT);
    pwm1__set_duty_cycle(PWM_MOTOR, 15);
  }
  if (callback_count == initial_callback_count + 200) {
    pwm1__set_duty_cycle(PWM_MOTOR, 15.9);
  }
  if (callback_count == initial_callback_count + 300) {
    pwm1__set_duty_cycle(PWM_MOTOR, 15);
  }
  if (callback_count == initial_callback_count + 400) {
    pwm1__set_duty_cycle(PWM_MOTOR, 14.1);
  }
  if (callback_count == initial_callback_count + 500) {
    pwm1__set_duty_cycle(PWM_MOTOR, 15);
  }
  if (callback_count == initial_callback_count + 550) {
    flag_start = true;
    set_motor_test_button_status(0);
  }
}