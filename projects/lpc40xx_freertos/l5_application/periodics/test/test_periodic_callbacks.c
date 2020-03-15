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
  can_bus_initializer__initialize_can1_Expect();
  ultrasonic_sensor_handler__initialize_sensors_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  gpio_s gpio = {};
  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);

  can_bus_initializer__reset_if_bus_off_can1_Expect();
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  gpio_s gpio = {};
  board_io__get_led1_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);

  sensor_can_handler__transmit_messages_10hz_Expect();
  periodic_callbacks__10Hz(0);
}

void test__periodic_callbacks__100Hz(void) {
  gpio_s gpio = {};
  board_io__get_led2_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);
  periodic_callbacks__100Hz(0);
}

void test__periodic_callbacks__1000Hz(void) {
  gpio_s gpio = {};
  board_io__get_led3_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);
  periodic_callbacks__1000Hz(0);
}