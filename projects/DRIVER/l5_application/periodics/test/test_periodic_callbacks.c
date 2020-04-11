#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockcan_bus_handler.h"
#include "Mockdriver_diagnostics.h"
#include "Mockgpio.h"
#include "Mocksjvalley_lcd.h"
// Include the source we wish to test
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  sjvalley_lcd__init_Expect();
  can_bus_handler__init_Expect();
  diagnostics_led_init_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  sjvalley_lcd__communication_init_Expect();
  sjvalley_lcd__send_line_IgnoreAndReturn(true);
  can_bus_handler__reset_if_bus_off_Expect();
  periodic_callbacks__1Hz(0);
}

// void test__periodic_callbacks__10Hz(void) {
//   gpio_s gpio = {};
//   board_io__get_led2_ExpectAndReturn(gpio);
//   gpio__toggle_Expect(gpio);
//   periodic_callbacks__10Hz(0);
// }

void test__periodic_callbacks__100Hz(void) {
  can_bus_handler__process_all_received_messages_in_100hz_Expect();
  can_bus_handler__transmit_message_in_100hz_Expect();
  can_bus_handler__manage_mia_100hz_Expect();
  periodic_callbacks__100Hz(0);
}