#include "periodic_callbacks.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "board_io.h"
#include "gpio.h"

#include "can_handler.h"
#include "motor_logic.h"
#include "motor_self_test.h"
#include "pwm1.h"
#include "speed_sensor.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  // This method is invoked once when the periodic tasks are created
  init_pwm();
  initialize_speed_sensor_and_test_button_interrupts();
  init_can_driver();
  rc_car_stop_state();
}

void periodic_callbacks__1Hz(uint32_t callback_count) {}

void periodic_callbacks__10Hz(uint32_t callback_count) {
  if (0 == callback_count % 5) {
    clear_rotations_in_windowtime();
  }
  can_bus_handler__transmit_message_in_10hz();
}

void periodic_callbacks__100Hz(uint32_t callback_count) {
  if (callback_count % 5 == 0) {
    if (get_motor_test_button_status()) {
      servo_and_dc_motor_tests(callback_count);
    } else {
      can_bus_handler__process_all_received_messages_in_20hz();
      can_bus_handler__manage_mia_20hz();
    }
  }
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {}