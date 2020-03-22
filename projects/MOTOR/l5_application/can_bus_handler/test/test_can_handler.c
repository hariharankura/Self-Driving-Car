#include "unity.h"

#include <stdbool.h>

#include "Mockcan_bus.h"
#include "Mockmotor_logic.h"

#include "can_handler.h"
#include "project.h"

void setUp(void) {}

void tearDown(void) {}

void test_init_can_driver(void) {
  can__init_ExpectAndReturn(can1, 100, 100, 100, NULL, NULL, true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(can1);
  init_can_driver();
}

bool can__rx_hijacked_mock(can__num_e can, can__msg_t *msg_ptr, uint32_t timeout_ms, int callback_count) {
  bool return_flag = false;
  printf("Callback_count = %d\n", callback_count);
  if (callback_count == 0) {
    msg_ptr->frame_fields.data_len = 2;
    msg_ptr->msg_id = 300;
    msg_ptr->data.bytes[0] = -2;
    msg_ptr->data.bytes[1] = 0;
    return_flag = true;
  }
  return return_flag;
}

void test_can_bus_handler__process_all_received_messages(void) {
  can__msg_t can_receive_msg = {};
  dbc_DRIVER_STEER_SPEED_s steer_data = {};
  can__rx_StubWithCallback(can__rx_hijacked_mock);
  motor_steer_logic_Expect(&steer_data);
  motor_steer_logic_IgnoreArg_steer_data();
  can_bus_handler__process_all_received_messages();
}
