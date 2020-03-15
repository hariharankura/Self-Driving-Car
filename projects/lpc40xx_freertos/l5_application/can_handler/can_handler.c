#include "can_handler.h"
#include "board_io.h"
#include "can_bus.h"
#include "gpio.h"
#include "motor_logic.h"
#include "project.h"

dbc_DRIVER_STEER_s steer_data = {};
dbc_MOTOR_SPEED_s speed_data = {};

void can_bus_handler__process_all_received_messages(void) {
  can__msg_t can_msg = {};

  // Receive all messages
  while (can__rx(can1, &can_msg, 0)) {
    const dbc_message_header_t header = {
        .message_id = can_msg.msg_id,
        .message_dlc = can_msg.frame_fields.data_len,
    };

    if (dbc_decode_DRIVER_STEER(&steer_data, header, can_msg.data.bytes)) {
      motor_steer_logic(&steer_data);
    }

    if (dbc_decode_MOTOR_SPEED(&speed_data, header, can_msg.data.bytes)) {
      motor_speed_logic(&speed_data);
    }
  }
}

void can_handler__manage_mia_10hz(void) {
  // We are in 10hz slot, so increment MIA counter by 100ms
  const uint32_t mia_increment_value = 100;

  if (dbc_service_mia_DRIVER_STEER(&steer_data, mia_increment_value)) {
    gpio__toggle(board_io__get_led0());
  }

  if (dbc_service_mia_MOTOR_SPEED(&speed_data, mia_increment_value)) {
    gpio__toggle(board_io__get_led1());
  }
}