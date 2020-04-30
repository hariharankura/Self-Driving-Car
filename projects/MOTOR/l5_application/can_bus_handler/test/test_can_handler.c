#include "unity.h"

#include <stdbool.h>

#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockgpio.h"
#include "Mockmotor_logic.h"
#include "Mockmotor_self_test.h"
#include "Mockspeed_sensor.h"

#include "can_handler.h"
#include "can_mia_config.c"
#include "project.h"

void setUp(void) {}

void tearDown(void) {}

void test_init_can_driver(void) {
  can__init_ExpectAndReturn(can1, 100, 100, 100, NULL, NULL, true);
  can__bypass_filter_accept_all_msgs_Expect();
  can__reset_bus_Expect(can1);
  gpio_s led;
  board_io__get_led0_ExpectAndReturn(led);
  gpio__set_Expect(led);
  board_io__get_led1_ExpectAndReturn(led);
  gpio__set_Expect(led);
  board_io__get_led2_ExpectAndReturn(led);
  gpio__set_Expect(led);
  board_io__get_led3_ExpectAndReturn(led);
  gpio__set_Expect(led);
  init_can_driver();
}

void test_can_dbc_handler__reset_if_bus_off(void) {
  can__is_bus_off_ExpectAndReturn(can1, true);
  can__reset_bus_Expect(can1);
  can_handler__reset_if_bus_off();
}

bool can__rx_steer_and_speed_hijacked_mock(can__num_e can, can__msg_t *msg_ptr, uint32_t timeout_ms,
                                           int callback_count) {
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

void test_can_bus_handler__process_all_received_messages_steer_and_speed(void) {
  can__msg_t can_receive_msg = {};
  dbc_DRIVER_STEER_SPEED_s steer_data = {};
  gpio_s test_led;
  can__rx_StubWithCallback(can__rx_steer_and_speed_hijacked_mock);
  motor_logic_Expect(&steer_data);
  motor_logic_IgnoreArg_steer_data();
  board_io__get_led0_ExpectAndReturn(test_led);
  gpio__set_Expect(test_led);
  can_bus_handler__process_all_received_messages_in_20hz();
}

bool can__rx_test_button_pressed_hijacked_mock(can__num_e can, can__msg_t *msg_ptr, uint32_t timeout_ms,
                                               int callback_count) {
  bool return_flag = false;
  printf("Callback_count = %d\n", callback_count);
  if (callback_count == 0) {
    msg_ptr->frame_fields.data_len = 1;
    msg_ptr->msg_id = 700;
    msg_ptr->data.bytes[0] = 1;
    return_flag = true;
  }
  return return_flag;
}

void test_can_bus_handler__process_all_received_messages_test_button_pressed(void) {
  can__msg_t can_receive_msg = {};
  dbc_TEST_BUTTON_s test_button;
  can__rx_StubWithCallback(can__rx_test_button_pressed_hijacked_mock);
  set_motor_test_button_status_Expect(1);
  can_bus_handler__process_all_received_messages_in_20hz();
}

bool can__rx_test_button_not_pressed_hijacked_mock(can__num_e can, can__msg_t *msg_ptr, uint32_t timeout_ms,
                                                   int callback_count) {
  bool return_flag = false;
  printf("Callback_count = %d\n", callback_count);
  if (callback_count == 0) {
    msg_ptr->frame_fields.data_len = 1;
    msg_ptr->msg_id = 700;
    msg_ptr->data.bytes[0] = 0;
    return_flag = true;
  }
  return return_flag;
}

void test_can_bus_handler__process_all_received_messages_test_button_not_pressed(void) {
  can__msg_t can_receive_msg = {};
  dbc_TEST_BUTTON_s test_button;
  can__rx_StubWithCallback(can__rx_test_button_not_pressed_hijacked_mock);
  can_bus_handler__process_all_received_messages_in_20hz();
}

void test_can_bus_handler__transmit_message(void) {
  can__msg_t send_msg = {};
  motor_speed_with_direction_ExpectAndReturn(0);
  get_pwm_forward_ExpectAndReturn(0);
  can__tx_ExpectAndReturn(can1, &send_msg, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  can_bus_handler__transmit_message_in_10hz();
}
