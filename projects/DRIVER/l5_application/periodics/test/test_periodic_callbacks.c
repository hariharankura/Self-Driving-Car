#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockdriver_diagnostics.h"
#include "Mockfull_can_bus_handler.h"
#include "Mockgpio.h"
#include "Mocksjvalley_lcd.h"
// Include the source we wish to test
#include "Mockdriver_logic.h"
#include "periodic_callbacks.h"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  sjvalley_lcd__init_Expect();
  full_can_bus_handler__init_Expect();
  diagnostics_led_init_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__1Hz(void) {
  sjvalley_lcd__communication_init_Expect();
  sjvalley_lcd__send_line_IgnoreAndReturn(true);
  full_can_bus_handler__reset_if_bus_off_Expect();
  periodic_callbacks__1Hz(0);
}

// void test__periodic_callbacks__10Hz(void) {
//   gpio_s gpio = {};
//   board_io__get_led2_ExpectAndReturn(gpio);
//   gpio__toggle_Expect(gpio);
//   periodic_callbacks__10Hz(0);
// }

void test__periodic_callbacks__100Hz(void) {
  for (uint32_t test_callback_count = 0; test_callback_count < 10; test_callback_count++) {
    if (!(test_callback_count % 5)) {
      full_can_bus_handler__transmit_message_in_20hz_Expect();
    }
    if (!(test_callback_count % 2)) {
      full_can_bus_handler__receive_messages_Expect();
      full_can_bus_handler__manage_mia_50hz_Expect();
    }
    periodic_callbacks__100Hz(test_callback_count);
  }
}