#include "sensor_can_handler.h"
#include "can_bus.h"
#include "led_handler.h"
#include "project.h"
#include "ultrasonic_sensor_handler.h"
#include <stdio.h>

void sensor_can_handler__transmit_messages_1hz(void) {
  dbc_SENSOR_USONARS_s ultrasonic_sensors_struct = {};
  uint16_t left_sensor = 0;
  uint16_t right_sensor = 0;
  uint16_t front_sensor = 0;
  uint16_t back_sensor = 0;

  left_sensor = ultrasonic_sensor_handler__get_filtered_sensor_value_left();
  right_sensor = ultrasonic_sensor_handler__get_filtered_sensor_value_right();
  front_sensor = ultrasonic_sensor_handler__get_filtered_sensor_value_front();
  back_sensor = ultrasonic_sensor_handler__get_filtered_sensor_value_back();
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor(left_sensor, right_sensor, front_sensor,
                                                                     back_sensor);

  ultrasonic_sensors_struct.SENSOR_USONARS_left = left_sensor;
  ultrasonic_sensors_struct.SENSOR_USONARS_right = right_sensor;
  ultrasonic_sensors_struct.SENSOR_USONARS_front = front_sensor;
  ultrasonic_sensors_struct.SENSOR_USONARS_back = back_sensor;
  printf("sensor values: left = %icm, front = %icm, right = %icm, back = %icm\n",
         ultrasonic_sensors_struct.SENSOR_USONARS_left, ultrasonic_sensors_struct.SENSOR_USONARS_front,
         ultrasonic_sensors_struct.SENSOR_USONARS_right, ultrasonic_sensors_struct.SENSOR_USONARS_back);

  can__msg_t sensor_can_msg = {};
  const dbc_message_header_t sensor_header =
      dbc_encode_SENSOR_USONARS(sensor_can_msg.data.bytes, &ultrasonic_sensors_struct);

  sensor_can_msg.msg_id = sensor_header.message_id;
  sensor_can_msg.frame_fields.data_len = sensor_header.message_dlc;

  can__tx(can1, &sensor_can_msg, 0);
}

void sensor_can_handler__transmit_messages_10hz(void) {
  dbc_SENSOR_USONARS_s ultrasonic_sensors_struct = {};
  uint16_t left_sensor = 0;
  uint16_t right_sensor = 0;
  uint16_t front_sensor = 0;
  uint16_t back_sensor = 0;

  left_sensor = ultrasonic_sensor_handler__get_filtered_sensor_value_left();
  right_sensor = ultrasonic_sensor_handler__get_filtered_sensor_value_right();
  front_sensor = ultrasonic_sensor_handler__get_filtered_sensor_value_front();
  back_sensor = ultrasonic_sensor_handler__get_filtered_sensor_value_back();
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor(left_sensor, right_sensor, front_sensor,
                                                                     back_sensor);

  ultrasonic_sensors_struct.SENSOR_USONARS_left = left_sensor;
  ultrasonic_sensors_struct.SENSOR_USONARS_right = right_sensor;
  ultrasonic_sensors_struct.SENSOR_USONARS_front = front_sensor;
  ultrasonic_sensors_struct.SENSOR_USONARS_back = back_sensor;
  printf("sensor values: left = %icm, front = %icm, right = %icm, back = %icm\n",
         ultrasonic_sensors_struct.SENSOR_USONARS_left, ultrasonic_sensors_struct.SENSOR_USONARS_front,
         ultrasonic_sensors_struct.SENSOR_USONARS_right, ultrasonic_sensors_struct.SENSOR_USONARS_back);

  can__msg_t sensor_can_msg = {};
  const dbc_message_header_t sensor_header =
      dbc_encode_SENSOR_USONARS(sensor_can_msg.data.bytes, &ultrasonic_sensors_struct);

  sensor_can_msg.msg_id = sensor_header.message_id;
  sensor_can_msg.frame_fields.data_len = sensor_header.message_dlc;

  can__tx(can1, &sensor_can_msg, 0);

  // OR
  // dbc_encode_and_send_SENSOR_USONARS(void *argument_for_dbc_send_can_message, &ultrasonic_sensors_struct);
}

void sensor_can_handler__transmit_messages_50hz(void) {
  dbc_SENSOR_USONARS_s ultrasonic_sensors_struct = {};
  uint16_t left_sensor = 0;
  uint16_t right_sensor = 0;
  uint16_t front_sensor = 0;
  uint16_t back_sensor = 0;

  left_sensor = ultrasonic_sensor_handler__get_filtered_sensor_value_left();
  right_sensor = ultrasonic_sensor_handler__get_filtered_sensor_value_right();
  front_sensor = ultrasonic_sensor_handler__get_filtered_sensor_value_front();
  back_sensor = ultrasonic_sensor_handler__get_filtered_sensor_value_back();
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor(left_sensor, right_sensor, front_sensor,
                                                                     back_sensor);

  ultrasonic_sensors_struct.SENSOR_USONARS_left = left_sensor;
  ultrasonic_sensors_struct.SENSOR_USONARS_right = right_sensor;
  ultrasonic_sensors_struct.SENSOR_USONARS_front = front_sensor;
  ultrasonic_sensors_struct.SENSOR_USONARS_back = back_sensor;

  can__msg_t sensor_can_msg = {};
  const dbc_message_header_t sensor_header =
      dbc_encode_SENSOR_USONARS(sensor_can_msg.data.bytes, &ultrasonic_sensors_struct);

  sensor_can_msg.msg_id = sensor_header.message_id;
  sensor_can_msg.frame_fields.data_len = sensor_header.message_dlc;

  can__tx(can1, &sensor_can_msg, 0);
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
      printf("received sensor data: left = %i, front = %i, right = %i, back = %i\n",
             decoded_sensor_cmd.SENSOR_USONARS_left, decoded_sensor_cmd.SENSOR_USONARS_front,
             decoded_sensor_cmd.SENSOR_USONARS_right, decoded_sensor_cmd.SENSOR_USONARS_back);
    }
  }
}