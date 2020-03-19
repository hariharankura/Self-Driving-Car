#include "unity.h"

#include <stdbool.h>

// Mock header file
#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockgpio.h"

#include "can_bus_handler.c"
#include "can_bus_mia_config.c"
#include "driver_logic.h"
#include "project.h"

const uint16_t test_CAN_BAUD_RATE = 100;
const uint16_t test_CAN_TRANSMIT_QUEUE_SIZE = 100;
const uint16_t test_CAN_RECEIVE_QUEUE_SIZE = 100;
const can__num_e test_CAN_BUS = can1;

gpio_s board_led_0 = {GPIO__PORT_2, 3};
gpio_s board_led_1 = {GPIO__PORT_1, 26};
gpio_s board_led_2 = {GPIO__PORT_1, 24};
gpio_s board_led_3 = {GPIO__PORT_1, 18};

void test_can_bus_handler__init(void) {
  gpio__set_Expect(board_led_0);
  gpio__set_Expect(board_led_1);
  gpio__set_Expect(board_led_2);
  gpio__set_Expect(board_led_3);
  board_io__get_led0_ExpectAndReturn(board_led_0);
  board_io__get_led1_ExpectAndReturn(board_led_1);
  board_io__get_led2_ExpectAndReturn(board_led_2);
  board_io__get_led3_ExpectAndReturn(board_led_3);
  can__init_ExpectAndReturn(test_CAN_BUS, test_CAN_BAUD_RATE, test_CAN_RECEIVE_QUEUE_SIZE, test_CAN_TRANSMIT_QUEUE_SIZE,
                            NULL, NULL, true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(test_CAN_BUS);
  can_bus_handler__init();
}

void test_can_dbc_handler__reset_if_bus_off(void) {
  can__is_bus_off_ExpectAndReturn(test_CAN_BUS, true);
  can__reset_bus_Expect(test_CAN_BUS);
  can_bus_handler__reset_if_bus_off();
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
  board_io__get_led0_ExpectAndReturn(board_led_0);
  gpio__set_Expect(board_led_0);
  can_bus_handler__process_all_received_messages_in_10hz();
}

void test_can_bus_handler__transmit_message(void) {
  can__msg_t send_msg = {};
  can__tx_ExpectAndReturn(test_CAN_BUS, &send_msg, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  can_bus_handler__transmit_message_in_10hz();
}

void test_can_bus_handler__manage_mia_10hz(void) {
  const uint32_t mia_increment_value = 100;
  // can_bus_handler__manage_mia_10hz();
  board_io__get_led0_ExpectAndReturn(board_led_0);
  gpio__reset_Expect(board_led_0);
  sensor_data.mia_info.mia_counter = dbc_mia_threshold_SENSOR_USONARS - mia_increment_value;
  can_bus_handler__manage_mia_10hz();
}