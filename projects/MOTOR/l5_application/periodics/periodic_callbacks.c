#include "periodic_callbacks.h"

#include "board_io.h"
#include "gpio.h"

#include "can_handler.h"
#include "motor_logic.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  // This method is invoked once when the periodic tasks are created
  init_can_driver();
  init_led();
}

void periodic_callbacks__1Hz(uint32_t callback_count) {}

void periodic_callbacks__10Hz(uint32_t callback_count) { can_bus_handler__process_all_received_messages(); }

void periodic_callbacks__100Hz(uint32_t callback_count) {}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {}