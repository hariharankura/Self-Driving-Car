#include "periodic_callbacks.h"

#include "board_io.h"
#include "can_bus_initializer.h"
#include "gpio.h"
#include "sensor_can_handler.h"
#include "ultrasonic_sensor_handler.h"

void periodic_callbacks__initialize(void) {
  gpio__set(board_io__get_led0());
  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__set(board_io__get_led3());
  can_bus_initializer__initialize_can1();
  ultrasonic_sensor_handler__initialize_sensors();
}

void periodic_callbacks__1Hz(uint32_t callback_count) { can_bus_initializer__reset_if_bus_off_can1(); }

void periodic_callbacks__10Hz(uint32_t callback_count) {
  sensor_can_handler__transmit_messages_10hz();
  // sensor_can_handler__handle_all_incoming_messages(); //for testing
}
void periodic_callbacks__100Hz(uint32_t callback_count) {}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {}