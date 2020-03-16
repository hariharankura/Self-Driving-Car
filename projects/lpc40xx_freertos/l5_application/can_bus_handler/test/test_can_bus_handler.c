#include "unity.h"

#include <stdbool.h>

#include "can_bus_handler.h"
#include "project.h"

// Mock header file
#include "Mockcan_bus.h"
#include "Mockdriver_logic.h"

const uint16_t test_CAN_BAUD_RATE = 100;
const uint16_t test_CAN_TRANSMIT_QUEUE_SIZE = 100;
const uint16_t test_CAN_RECEIVE_QUEUE_SIZE = 100;
const can__num_e test_CAN_BUS = can1;

void test_can_bus_handler__init(void) {
  can__init_ExpectAndReturn(test_CAN_BUS, test_CAN_BAUD_RATE, test_CAN_RECEIVE_QUEUE_SIZE, test_CAN_TRANSMIT_QUEUE_SIZE,
                            NULL, NULL, true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(can1);
  can_bus_handler__init();
}

bool can__rx_hijacked_mock(can__num_e can, can__msg_t *msg_ptr, uint32_t timeout_ms, int callback_count) {
  bool return_flag = false;
  printf("Callback_count = %d\n", callback_count);
  if (callback_count == 0) {
    msg_ptr->frame_fields.data_len = 5;
    msg_ptr->msg_id = 200;
    msg_ptr->data.bytes[0] = 4;
    msg_ptr->data.bytes[1] = ((uint8_t)3 << 2);
    msg_ptr->data.bytes[2] = ((uint8_t)2 << 4);
    msg_ptr->data.bytes[3] = ((uint8_t)1 << 2);
    return_flag = true;
  }
  return return_flag;
}

void test_can_bus_handler__process_all_received_messages(void) {
  can__msg_t can_receive_msg = {};
  dbc_SENSOR_USONARS_s sensor_data = {};
  can__rx_StubWithCallback(can__rx_hijacked_mock);
  driver_logic__process_ultrasonic_sensors_data_Expect(sensor_data);
  driver_logic__process_ultrasonic_sensors_data_IgnoreArg_sensor_data();
  can_bus_handler__process_all_received_messages();
}

void test_can_bus_handler__transmit_message(void) {
  dbc_DRIVER_STEER_s steer_info = {};
  dbc_MOTOR_SPEED_s speed_info = {};
  can__msg_t send_msg = {};
  driver_logic__check_if_steer_data_to_send_ExpectAndReturn(&steer_info, true);
  driver_logic__check_if_speed_data_to_send_ExpectAndReturn(&speed_info, false);
  can__tx_ExpectAndReturn(test_CAN_BUS, &send_msg, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  can_bus_handler__transmit_message();
}