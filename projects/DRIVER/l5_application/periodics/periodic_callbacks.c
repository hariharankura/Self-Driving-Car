#include "periodic_callbacks.h"

#include "board_io.h"
#include "can_bus_handler.h"
#include "driver_diagnostics.h"
#include "gpio.h"

/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
void periodic_callbacks__initialize(void) {
  // This method is invoked once when the periodic tasks are created
  diagnostics_led_init();
  can_bus_handler__init();
}

void periodic_callbacks__1Hz(uint32_t callback_count) { can_bus_handler__reset_if_bus_off(); }

void periodic_callbacks__10Hz(uint32_t callback_count) {
  can_bus_handler__process_all_received_messages_in_10hz();
  can_bus_handler__manage_mia_10hz();
  can_bus_handler__transmit_message_in_10hz();
}

void periodic_callbacks__100Hz(uint32_t callback_count) {
  // gpio__toggle(board_io__get_led2());
  // Add your code here
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {
  // gpio__toggle(board_io__get_led3());
  // Add your code here
}