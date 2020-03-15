#include "unity.h"

#include <stdbool.h>

#include "Mockcan_bus.h"

#include "can_bus_initializer.c"

void setUp(void) {}

void tearDown(void) {}

void test_init_can_driver(void) {
  can__init_ExpectAndReturn(can1, 500, 100, 100, NULL, NULL, true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(can1);
  init_can_driver();
}