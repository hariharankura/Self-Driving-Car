#include "periodic_callbacks.h"

#include "bridge_can_handler.h"
#include "bridge_controller_handler.h"
#include "can_bus_initializer.h"
#include "led_handler.h"
#include "sensor_can_handler.h"
#include "ultrasonic_sensor_handler.h"

void periodic_callbacks__initialize(void) {
  led_handler__turn_off_all_sjtwo_leds();
  can_bus_initializer__initialize_can1();
  ultrasonic_sensor_handler__initialize_sensors();
  bridge_controller_handler__initialize_bluetooth_module();
}

void periodic_callbacks__1Hz(uint32_t callback_count) { can_bus_initializer__reset_if_bus_off_can1(); }

void periodic_callbacks__10Hz(uint32_t callback_count) { bridge_can_handler__transmit_messages_10hz(); }

void periodic_callbacks__50Hz(uint32_t callback_count) { sensor_can_handler__transmit_messages_50hz(); }

void periodic_callbacks__100Hz(uint32_t callback_count) {}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {}