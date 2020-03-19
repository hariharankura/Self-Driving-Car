#include "unity.h"

#include "Mockboard_io.h"
#include "Mockcan_bus_initializer.h"
#include "Mockgpio.h"
#include "Mocksensor_can_handler.h"
#include "Mockultrasonic_sensor_handler.h"

#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  gpio_s led0, led1, led2, led3;
  board_io__get_led0_ExpectAndReturn(led0);
  gpio__set_Expect(led0);
  board_io__get_led1_ExpectAndReturn(led1);
  gpio__set_Expect(led1);
  board_io__get_led2_ExpectAndReturn(led2);
  gpio__set_Expect(led2);
  board_io__get_led3_ExpectAndReturn(led3);
  gpio__set_Expect(led3);

  can_bus_initializer__initialize_can1_Expect();
  ultrasonic_sensor_handler__initialize_sensors_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  can_bus_initializer__reset_if_bus_off_can1_Expect();
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  sensor_can_handler__transmit_messages_10hz_Expect();
  // sensor_can_handler__handle_all_incoming_messages_Expect(); //for testing
  periodic_callbacks__10Hz(0);
}

void test__periodic_callbacks__100Hz(void) { periodic_callbacks__100Hz(0); }

void test__periodic_callbacks__1000Hz(void) { periodic_callbacks__1000Hz(0); }