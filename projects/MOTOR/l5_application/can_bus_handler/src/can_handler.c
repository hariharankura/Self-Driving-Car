#include "can_handler.h"
#include "board_io.h"
#include "can_bus.h"
#include "gpio.h"
#include "motor_logic.h"
#include "project.h"
#include "speed_sensor.h"

void init_can_driver(void) {
  can__init(can1, 100, 100, 100, NULL, NULL);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(can1);
}

void can_bus_handler__process_all_received_messages_in_10hz(void) {
  can__msg_t can_msg = {};
  dbc_DRIVER_STEER_SPEED_s steer_data = {};

  // Receive all messages
  while (can__rx(can1, &can_msg, 0)) {
    const dbc_message_header_t header = {
        .message_id = can_msg.msg_id,
        .message_dlc = can_msg.frame_fields.data_len,
    };

    if (dbc_decode_DRIVER_STEER_SPEED(&steer_data, header, can_msg.data.bytes)) {
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

bool dbc_send_can_message(void *argument, uint32_t message_id, const uint8_t bytes[8], uint8_t dlc) {
  can__msg_t send_msg = {};
  send_msg.msg_id = message_id;
  send_msg.frame_fields.data_len = dlc;
  memcpy(send_msg.data.bytes, bytes, dlc);
  return can__tx(can1, &send_msg, 0);
}