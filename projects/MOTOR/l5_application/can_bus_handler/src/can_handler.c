#include "can_handler.h"

#include <stdio.h>

#include "board_io.h"
#include "can_bus.h"
#include "gpio.h"
#include "motor_logic.h"
#include "project.h"
#include "speed_sensor.h"

static const uint16_t CAN_BAUD_RATE = 100;
static const uint16_t CAN_TRANSMIT_QUEUE_SIZE = 100;
static const uint16_t CAN_RECEIVE_QUEUE_SIZE = 100;
static const can__num_e CAN_BUS = can1;

dbc_DRIVER_STEER_SPEED_s steer_data = {};

static void can_handler__board_led_reset(void) {
  gpio__set(board_io__get_led0());
  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__set(board_io__get_led3());
}

void init_can_driver(void) {
  can__init(CAN_BUS, CAN_BAUD_RATE, CAN_TRANSMIT_QUEUE_SIZE, CAN_RECEIVE_QUEUE_SIZE, NULL, NULL);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(can1);
  can_handler__board_led_reset();
}

void can_handler__reset_if_bus_off(void) {
  if (can__is_bus_off(CAN_BUS)) {
    printf("Bus off...Resetting can bus\n");
    can__reset_bus(CAN_BUS);
  }
}

void can_bus_handler__process_all_received_messages_in_20hz(void) {
  can__msg_t can_msg = {};

  // Receive all messages
  while (can__rx(CAN_BUS, &can_msg, 0)) {
    const dbc_message_header_t header = {
        .message_id = can_msg.msg_id,
        .message_dlc = can_msg.frame_fields.data_len,
    };

    if (dbc_decode_DRIVER_STEER_SPEED(&steer_data, header, can_msg.data.bytes)) {
      // printf("Received msg\n");
      gpio__set(board_io__get_led0());
      motor_logic(&steer_data);
    }
  }
}

void can_bus_handler__transmit_message_in_10hz(void) {
  dbc_MOTOR_SPEED_s motor_speed = {};
  can__msg_t can_transmit_msg = {};
  motor_speed.MOTOR_SPEED_info = get_mph();
  dbc_encode_and_send_MOTOR_SPEED(&can_transmit_msg, &motor_speed);
}

void can_bus_handler__manage_mia_20hz(void) {
  const uint32_t mia_increment_value = 100;
  if (dbc_service_mia_DRIVER_STEER_SPEED(&steer_data, mia_increment_value)) {
    motor_logic(&steer_data);
    gpio__reset(board_io__get_led0());
  }
}

bool dbc_send_can_message(void *argument, uint32_t message_id, const uint8_t bytes[8], uint8_t dlc) {
  can__msg_t send_msg = {};
  send_msg.msg_id = message_id;
  send_msg.frame_fields.data_len = dlc;
  memcpy(send_msg.data.bytes, bytes, dlc);
  return can__tx(CAN_BUS, &send_msg, 0);
}