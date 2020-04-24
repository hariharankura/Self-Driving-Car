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
<<<<<<< HEAD
  /*printf("bridge data: lat = %f, long = %f\n", bridge_struct.BRIDGE_GPS_latitude,
   * *bridge_struct.BRIDGE_GPS_longitude);*/
=======
  printf("bridge data: lat = %f, long = %f\n", bridge_struct.BRIDGE_GPS_latitude, bridge_struct.BRIDGE_GPS_longitude);
>>>>>>> new bridge changes

  can__msg_t bridge_can_msg = {};
  const dbc_message_header_t bridge_header = dbc_encode_BRIDGE_GPS(bridge_can_msg.data.bytes, &bridge_struct);

  bridge_can_msg.msg_id = bridge_header.message_id;
  bridge_can_msg.frame_fields.data_len = bridge_header.message_dlc;

  can__tx(can1, &bridge_can_msg, 0);
}

void bridge_can_handler__transmit_start_stop_condition(void) {
  dbc_CAR_ACTION_s car_action_struct;

  car_action_struct.CAR_ACTION_cmd = bridge_controller_handler__get_start_stop_condition();
  // printf("Command = %d\n", car_action_struct.CAR_ACTION_cmd);
  can__msg_t car_action_can_msg = {};
  const dbc_message_header_t car_action_header =
      dbc_encode_CAR_ACTION(car_action_can_msg.data.bytes, &car_action_struct);

  car_action_can_msg.msg_id = car_action_header.message_id;
  car_action_can_msg.frame_fields.data_len = car_action_header.message_dlc;

  can__tx(can1, &car_action_can_msg, 0);
}

void bridge_can_handler__handle_all_incoming_messages(void) {
  dbc_MOTOR_SPEED_s motor_speed_message = {};
  dbc_GEO_COMPASS_s geo_compass_message = {};
  dbc_DRIVER_STEER_SPEED_s driver_steer_message = {};

  can__msg_t can_msg = {};

  while (can__rx(can1, &can_msg, 0)) {
    const dbc_message_header_t header = {
        .message_id = can_msg.msg_id,
        .message_dlc = can_msg.frame_fields.data_len,
    };

<<<<<<< HEAD
    if (dbc_decode_MOTOR_SPEED(&motor_speed_message, header, can_msg.data.bytes)) {
      debug_motor_speed = motor_speed_message.MOTOR_SPEED_info;
      debug_motor_speed_pwm = motor_speed_message.MOTOR_SPEED_pwm;
    }

    if (dbc_decode_GEO_COMPASS(&geo_compass_message, header, can_msg.data.bytes)) {
      debug_geo_compass_current_heading = geo_compass_message.GEO_COMPASS_current_heading;
      debug_geo_compass_destination_heading = geo_compass_message.GEO_COMPASS_desitination_heading;
      debug_geo_compass_distance = geo_compass_message.GEO_COMPASS_distance;
    }

    if (dbc_decode_DRIVER_STEER_SPEED(&driver_steer_message, header, can_msg.data.bytes)) {
      debug_steer_move_speed = driver_steer_message.DRIVER_STEER_move_speed;
      debug_steer_direction = driver_steer_message.DRIVER_STEER_direction;
=======
    if (dbc_decode_BRIDGE_GPS(&decoded_bridge_cmd, header, bridge_can_msg.data.bytes)) {
      printf("received bridge data: latitude = %f, longitude = %f\n", decoded_bridge_cmd.BRIDGE_GPS_latitude,
             decoded_bridge_cmd.BRIDGE_GPS_longitude);
>>>>>>> new bridge changes
    }
  }
}

void bridge_can_handler__transmit_start_stop_condition(void) {
  dbc_CAR_ACTION_s car_action_struct;

  car_action_struct.CAR_ACTION_cmd = bridge_controller_handler__get_start_stop_condition();
  // printf("Command = %d\n", car_action_struct.CAR_ACTION_cmd);
  can__msg_t car_action_can_msg = {};
  const dbc_message_header_t car_action_header =
      dbc_encode_CAR_ACTION(car_action_can_msg.data.bytes, &car_action_struct);

  car_action_can_msg.msg_id = car_action_header.message_id;
  car_action_can_msg.frame_fields.data_len = car_action_header.message_dlc;

  can__tx(can1, &car_action_can_msg, 0);
}
