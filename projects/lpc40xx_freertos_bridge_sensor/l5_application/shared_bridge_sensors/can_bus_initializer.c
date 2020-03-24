#include <stdio.h>

#include "can_bus.h"
#include "can_bus_initializer.h"

void can_bus_initializer__initialize_can1(void) {
  can__init(can1, 100, 10, 10, (can_void_func_t)NULL, (can_void_func_t)NULL);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(can1);
}

void can_bus_initializer__reset_if_bus_off_can1(void) {
  if (can__is_bus_off(can1)) {
    can__reset_bus(can1);
  }
}

void can_bus_initializer__initialize_can2(void) {
  can__init(can2, 100, 10, 10, (can_void_func_t)1, (can_void_func_t)1);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(can2);
}

void can_bus_initializer__reset_if_bus_off_can2(void) {
  if (can__is_bus_off(can2)) {
    can__reset_bus(can2);
  }
}