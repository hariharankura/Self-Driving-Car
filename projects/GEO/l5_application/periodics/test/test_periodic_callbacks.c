#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockcan_bus_handler.h"
#include "Mockcompass.h"
#include "Mockgpio.h"
#include "Mockgps.h"
// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  gps__init_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  gpio_s gpio = {};
  board_io__get_led0_ExpectAndReturn(gpio);
  gpio__toggle_Expect(gpio);
  periodic_callbacks__1Hz(0);
}

void test__periodic_callbacks__10Hz(void) {
  // gps_coordinates_t temp_coordinates;
  // gps__run_once_Ignore();
  // gps__get_coordinates_ExpectAndReturn(temp_coordinates);
  compass__read_current_gps_coordinate_Expect();
  can_bus_handler__transmit_message_in_10hz_Expect();
  can_bus_handler__process_all_received_messages_in_10hz_Expect();
  periodic_callbacks__10Hz(0);
}