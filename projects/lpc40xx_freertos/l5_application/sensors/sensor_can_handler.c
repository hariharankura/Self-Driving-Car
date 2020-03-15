#include "sensor_can_handler.h"
#include "can_bus.h"
#include "project.h"
#include "ultrasonic_sensor_handler.h"

void sensor_can_handler__transmit_messages_10hz(void) {
  dbc_SENSOR_USONARS_s ultrasonic_sensors_struct = {};

  // TODO: populate ultrasonic_sensors_struct members with real sensor values
  ultrasonic_sensors_struct.SENSOR_USONARS_left = ultrasonic_sensor_handler__get_sensor_value_left();
  ultrasonic_sensors_struct.SENSOR_USONARS_right = ultrasonic_sensor_handler__get_sensor_value_right();
  ultrasonic_sensors_struct.SENSOR_USONARS_front = ultrasonic_sensor_handler__get_sensor_value_front();
  ultrasonic_sensors_struct.SENSOR_USONARS_back = ultrasonic_sensor_handler__get_sensor_value_back();

  can__msg_t sensor_can_msg = {};
  const dbc_message_header_t sensor_header =
      dbc_encode_SENSOR_USONARS(sensor_can_msg.data.bytes, &ultrasonic_sensors_struct);

  sensor_can_msg.msg_id = sensor_header.message_id;
  sensor_can_msg.frame_fields.data_len = sensor_header.message_dlc;

  can__tx(can1, &sensor_can_msg, 0);

  // OR
  // dbc_encode_and_send_SENSOR_USONARS(void *argument_for_dbc_send_can_message, &ultrasonic_sensors_struct);
}