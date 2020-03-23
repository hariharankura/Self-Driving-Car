#include "periodic_callbacks.h"

#include "bridge_can_handler.h"
#include "bridge_controller_handler.h"
#include "can_bus_initializer.h"
#include "led_handler.h"
#include "sensor_can_handler.h"
#include "ultrasonic_sensor_handler.h"

void periodic_callbacks__initialize(void) {
  can_bus_initializer__initialize_can1();
  ultrasonic_sensor_handler__initialize_sensors();
  bridge_controller_handler__initialize_bluetooth_module();
}

void periodic_callbacks__1Hz(uint32_t callback_count) { can_bus_initializer__reset_if_bus_off_can1(); }

void periodic_callbacks__10Hz(uint32_t callback_count) {
  sensor_can_handler__transmit_messages_10hz();
  // sensor_can_handler__handle_all_incoming_messages(); //for testing
  bridge_can_handler__transmit_messages_10hz();
  // bridge_can_handler__handle_all_incoming_messages(); //for testing
}
void periodic_callbacks__100Hz(uint32_t callback_count) {}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {}