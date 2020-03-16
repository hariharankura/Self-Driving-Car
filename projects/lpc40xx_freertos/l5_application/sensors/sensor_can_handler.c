#include "sensor_can_handler.h"
#include "can_bus.h"
#include "project.h"
#include "ultrasonic_sensor_handler.h"
#include <stdio.h>

void sensor_can_handler__transmit_messages_10hz(void) {
  dbc_SENSOR_USONARS_s ultrasonic_sensors_struct = {};

  ultrasonic_sensors_struct.SENSOR_USONARS_left = ultrasonic_sensor_handler__get_sensor_value_left();
  ultrasonic_sensors_struct.SENSOR_USONARS_right = ultrasonic_sensor_handler__get_sensor_value_right();
  ultrasonic_sensors_struct.SENSOR_USONARS_front = ultrasonic_sensor_handler__get_sensor_value_front();
  ultrasonic_sensors_struct.SENSOR_USONARS_back = ultrasonic_sensor_handler__get_sensor_value_back();
  /*printf("sensor values: left: %i, right: %i, front: %i, back: %i\n", ultrasonic_sensors_struct.SENSOR_USONARS_left,
  ultrasonic_sensors_struct.SENSOR_USONARS_right, ultrasonic_sensors_struct.SENSOR_USONARS_front,
  ultrasonic_sensors_struct.SENSOR_USONARS_back);*/

  can__msg_t sensor_can_msg = {};
  const dbc_message_header_t sensor_header =
      dbc_encode_SENSOR_USONARS(sensor_can_msg.data.bytes, &ultrasonic_sensors_struct);

  sensor_can_msg.msg_id = sensor_header.message_id;
  sensor_can_msg.frame_fields.data_len = sensor_header.message_dlc;

  can__tx(can1, &sensor_can_msg, 0);

  // OR
  // dbc_encode_and_send_SENSOR_USONARS(void *argument_for_dbc_send_can_message, &ultrasonic_sensors_struct);
}

void sensor_can_handler__handle_all_incoming_messages(void) {
  dbc_SENSOR_USONARS_s decoded_sensor_cmd = {};
  can__msg_t sensor_can_msg = {};

  while (can__rx(can1, &sensor_can_msg, 0)) {
    const dbc_message_header_t header = {
        .message_id = sensor_can_msg.msg_id,
        .message_dlc = sensor_can_msg.frame_fields.data_len,
    };

    if (dbc_decode_SENSOR_USONARS(&decoded_sensor_cmd, header, sensor_can_msg.data.bytes)) {
      printf("received sensor data: left: %i, right: %i, front: %i, back: %i\n", decoded_sensor_cmd.SENSOR_USONARS_left,
             decoded_sensor_cmd.SENSOR_USONARS_right, decoded_sensor_cmd.SENSOR_USONARS_front,
             decoded_sensor_cmd.SENSOR_USONARS_back);
    }
  }
}