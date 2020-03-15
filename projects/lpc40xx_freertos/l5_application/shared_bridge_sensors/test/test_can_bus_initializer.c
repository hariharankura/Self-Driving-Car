#include "unity.h"

#include "Mockcan_bus.h"

#include "can_bus_initializer.h"

void test_can_bus_initializer__initialize_can1(void) {
  can__init_ExpectAnyArgsAndReturn(true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_ExpectAnyArgs();

  can_bus_initializer__initialize_can1();
}

void test_can_bus_initializer__reset_if_bus_off_can1_returns_true(void) {
  can__is_bus_off_ExpectAnyArgsAndReturn(true);
  can__reset_bus_ExpectAnyArgs();

  can_bus_initializer__reset_if_bus_off_can1();
}

void test_can_bus_initializer__reset_if_bus_off_can1_returns_false(void) {
  can__is_bus_off_ExpectAnyArgsAndReturn(false);

  can_bus_initializer__reset_if_bus_off_can1();
}

void test_can_bus_initializer__initialize_can2(void) {
  can__init_ExpectAnyArgsAndReturn(true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_ExpectAnyArgs();

  can_bus_initializer__initialize_can2();
}

void test_can_bus_initializer__reset_if_bus_off_can2_returns_true(void) {
  can__is_bus_off_ExpectAnyArgsAndReturn(true);
  can__reset_bus_ExpectAnyArgs();

  can_bus_initializer__reset_if_bus_off_can2();
}

void test_can_bus_initializer__reset_if_bus_off_can2_returns_false(void) {
  can__is_bus_off_ExpectAnyArgsAndReturn(false);

  can_bus_initializer__reset_if_bus_off_can2();
}