#include "bridge_can_handler.h"
#include "bridge_controller_handler.h"
#include "can_bus.h"
#include "project.h"
#include <stdio.h>

void bridge_can_handler__transmit_messages_10hz(void) {
  dbc_BRIDGE_GPS_s bridge_struct = {};
  float latitude = 0;
  float longitude = 0;

  bridge_controller_handler__get_destination_coordinates(&latitude, &longitude);
  bridge_struct.BRIDGE_GPS_latitude = latitude;
  bridge_struct.BRIDGE_GPS_longitude = longitude;
  printf("bridge data: lat = %f, long = %f\n", bridge_struct.BRIDGE_GPS_latitude, bridge_struct.BRIDGE_GPS_longitude);

  can__msg_t bridge_can_msg = {};
  const dbc_message_header_t bridge_header = dbc_encode_BRIDGE_GPS(bridge_can_msg.data.bytes, &bridge_struct);

  bridge_can_msg.msg_id = bridge_header.message_id;
  bridge_can_msg.frame_fields.data_len = bridge_header.message_dlc;

  can__tx(can1, &bridge_can_msg, 0);
}

void bridge_can_handler__handle_all_incoming_messages(void) {
  dbc_BRIDGE_GPS_s decoded_bridge_cmd = {};
  can__msg_t bridge_can_msg = {};

  while (can__rx(can1, &bridge_can_msg, 0)) {
    const dbc_message_header_t header = {
        .message_id = bridge_can_msg.msg_id,
        .message_dlc = bridge_can_msg.frame_fields.data_len,
    };

    if (dbc_decode_BRIDGE_GPS(&decoded_bridge_cmd, header, bridge_can_msg.data.bytes)) {
      printf("received bridge data: latitude = %f, longitude = %f\n", decoded_bridge_cmd.BRIDGE_GPS_latitude,
             decoded_bridge_cmd.BRIDGE_GPS_longitude);
    }
  }
}