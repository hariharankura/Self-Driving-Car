#include "periodic_callbacks.h"
#include "stdint.h"

#include "board_io.h"
#include "gpio.h"

#include "can_handler.h"
#include "motor_logic.h"
#include "pwm1.h"
#include "speed_sensor.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  // This method is invoked once when the periodic tasks are created
  init_can_driver();
  init_pwm();
  init_speed_sensor();
}

void periodic_callbacks__1Hz(uint32_t callback_count) {}

void periodic_callbacks__10Hz(uint32_t callback_count) {

  // uint8_t windowtime = get_windowtime();
  uint8_t windowtime = 5;

  can_bus_handler__process_all_received_messages_in_10hz();

  if (0 == callback_count % (10 * windowtime)) {
    gpio__toggle(board_io__get_led0());
    clear_rotations_in_windowtime();
  }

  can_bus_handler__transmit_message_in_10hz();
}

void periodic_callbacks__100Hz(uint32_t callback_count) {
  pwm1__set_duty_cycle(PWM1__2_0, 10);
  pwm1__set_duty_cycle(PWM1__2_1, 10);
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {}