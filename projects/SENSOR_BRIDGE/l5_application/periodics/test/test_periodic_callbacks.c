#include "unity.h"

#include "Mockbridge_can_handler.h"
#include "Mockbridge_controller_handler.h"
#include "Mockcan_bus_initializer.h"
#include "Mockled_handler.h"
#include "Mocksensor_can_handler.h"
#include "Mockultrasonic_sensor_handler.h"

#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  led_handler__turn_off_all_sjtwo_leds_Expect();
  can_bus_initializer__initialize_can1_Expect();
  ultrasonic_sensor_handler__initialize_sensors_Expect();
  bridge_controller_handler__initialize_bluetooth_module_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  can_bus_initializer__reset_if_bus_off_can1_Expect();
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  // bridge_can_handler__transmit_messages_10hz_Expect();
  bridge_controller_handler__get_start_stop_condition_ExpectAndReturn(true);
  bridge_controller_handler__send_debug_info_Expect();
  bridge_can_handler__handle_all_incoming_messages_Expect();
  periodic_callbacks__10Hz(0);
}

void test__periodic_callbacks__50Hz(void) {
  sensor_can_handler__transmit_messages_50hz_Expect();
  periodic_callbacks__50Hz(0);
}

void test__periodic_callbacks__100Hz(void) { periodic_callbacks__100Hz(0); }

void test__periodic_callbacks__1000Hz(void) { periodic_callbacks__1000Hz(0); }