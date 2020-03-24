#include "unity.h"

#include "bridge_controller_handler.h"

void test_bridge_controller_handler__initialize_bluetooth_module(void) {
  bridge_controller_handler__initialize_bluetooth_module();
}

void test_bridge_controller_handler__get_destination_coordinates(void) {
  float latitude = 0;
  float longitude = 0;

  bridge_controller_handler__get_destination_coordinates(&latitude, &longitude);

  TEST_ASSERT_EQUAL_FLOAT(112.034, latitude);
  TEST_ASSERT_EQUAL_FLOAT(4509.1904, longitude);
}