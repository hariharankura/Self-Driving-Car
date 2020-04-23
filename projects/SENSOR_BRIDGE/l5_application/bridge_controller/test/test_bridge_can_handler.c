#include "unity.h"

#include "Mockbridge_controller_handler.h"
#include "Mockcan_bus.h"
#include "project.h"

#include "bridge_can_handler.h"

float debug_motor_speed = 0.0f;
float debug_motor_speed_pwm = 0.0f;
float debug_geo_compass_current_heading;
float debug_geo_compass_destination_heading;
float debug_geo_compass_distance;
int debug_steer_move_speed;
DRIVER_STEER_direction_e debug_steer_direction;

void test_bridge_can_handler__transmit_messages_10hz(void) {
  dbc_BRIDGE_GPS_s bridge_struct = {};
  float latitude = 0;
  float longitude = 0;

  bridge_controller_handler__get_destination_coordinates_Expect(&latitude, &longitude);
  bridge_struct.BRIDGE_GPS_latitude = latitude;
  bridge_struct.BRIDGE_GPS_longitude = longitude;

  can__msg_t bridge_can_msg = {};
  const dbc_message_header_t bridge_header = dbc_encode_BRIDGE_GPS(bridge_can_msg.data.bytes, &bridge_struct);

  bridge_can_msg.msg_id = bridge_header.message_id;
  bridge_can_msg.frame_fields.data_len = bridge_header.message_dlc;

  can__tx_ExpectAnyArgsAndReturn(true);

  bridge_can_handler__transmit_messages_10hz();
}

void test_bridge_can_handler__handle_all_incoming_messages(void) {
  dbc_MOTOR_SPEED_s motor_speed_message = {};
  can__msg_t can_msg = {};

  can__rx_ExpectAnyArgsAndReturn(true);
  const dbc_message_header_t header = {
      .message_id = can_msg.msg_id,
      .message_dlc = can_msg.frame_fields.data_len,
  };

  dbc_decode_MOTOR_SPEED(&motor_speed_message, header, can_msg.data.bytes);
  can__rx_ExpectAnyArgsAndReturn(false);
  bridge_can_handler__handle_all_incoming_messages();
}