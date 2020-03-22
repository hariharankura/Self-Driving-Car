#include "can_handler.h"
#include "board_io.h"
#include "can_bus.h"
#include "gpio.h"
#include "motor_logic.h"
#include "project.h"

void init_can_driver(void) {
  can__init(can1, 100, 100, 100, NULL, NULL);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(can1);
}

void can_bus_handler__process_all_received_messages(void) {
  can__msg_t can_msg = {};
  dbc_DRIVER_STEER_SPEED_s steer_data = {};

  // Receive all messages
  while (can__rx(can1, &can_msg, 0)) {
    const dbc_message_header_t header = {
        .message_id = can_msg.msg_id,
        .message_dlc = can_msg.frame_fields.data_len,
    };

    if (dbc_decode_DRIVER_STEER_SPEED(&steer_data, header, can_msg.data.bytes)) {
      motor_steer_logic(&steer_data);
    }
  }
}