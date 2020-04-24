#include "unity.h"

#include "Mockcan_bus.h"
#include "Mockled_handler.h"
#include "Mockultrasonic_sensor_handler.h"
#include "project.h"

#include "sensor_can_handler.h"

void test_sensor_can_handler__transmit_messages_1hz(void) {
  dbc_SENSOR_USONARS_s ultrasonic_sensors_struct = {};

  uint16_t sensor1, sensor2, sensor3, sensor4;

  ultrasonic_sensor_handler__get_filtered_sensor_value_left_ExpectAndReturn(sensor1);
  ultrasonic_sensor_handler__get_filtered_sensor_value_right_ExpectAndReturn(sensor2);
  ultrasonic_sensor_handler__get_filtered_sensor_value_front_ExpectAndReturn(sensor3);
  ultrasonic_sensor_handler__get_filtered_sensor_value_back_ExpectAndReturn(sensor4);
  ultrasonic_sensor_handler__set_all_sensor_values_Expect(sensor1, sensor2, sensor3, sensor4);
<<<<<<< HEAD
=======
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor_Expect(sensor1, sensor2, sensor3, sensor4);
>>>>>>> sensor module changes

  can__msg_t sensor_can_msg = {};
  const dbc_message_header_t sensor_header =
      dbc_encode_SENSOR_USONARS(sensor_can_msg.data.bytes, &ultrasonic_sensors_struct);

  sensor_can_msg.msg_id = sensor_header.message_id;
  sensor_can_msg.frame_fields.data_len = sensor_header.message_dlc;

  can__tx_ExpectAnyArgsAndReturn(true);

  sensor_can_handler__transmit_messages_1hz();
}

void test_sensor_can_handler__transmit_messages_10hz(void) {
  dbc_SENSOR_USONARS_s ultrasonic_sensors_struct = {};

  uint16_t sensor1, sensor2, sensor3, sensor4;

  ultrasonic_sensor_handler__get_filtered_sensor_value_left_ExpectAndReturn(sensor1);
  ultrasonic_sensor_handler__get_filtered_sensor_value_right_ExpectAndReturn(sensor2);
  ultrasonic_sensor_handler__get_filtered_sensor_value_front_ExpectAndReturn(sensor3);
  ultrasonic_sensor_handler__get_filtered_sensor_value_back_ExpectAndReturn(sensor4);
  ultrasonic_sensor_handler__set_all_sensor_values_Expect(sensor1, sensor2, sensor3, sensor4);
<<<<<<< HEAD
=======
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor_Expect(sensor1, sensor2, sensor3, sensor4);
>>>>>>> sensor module changes

  can__msg_t sensor_can_msg = {};
  const dbc_message_header_t sensor_header =
      dbc_encode_SENSOR_USONARS(sensor_can_msg.data.bytes, &ultrasonic_sensors_struct);

  sensor_can_msg.msg_id = sensor_header.message_id;
  sensor_can_msg.frame_fields.data_len = sensor_header.message_dlc;

  can__tx_ExpectAnyArgsAndReturn(true);

  sensor_can_handler__transmit_messages_10hz();
}

void test_sensor_can_handler__transmit_messages_50hz(void) {
  dbc_SENSOR_USONARS_s ultrasonic_sensors_struct = {};

  uint16_t sensor1, sensor2, sensor3, sensor4;

  ultrasonic_sensor_handler__get_filtered_sensor_value_left_ExpectAndReturn(sensor1);
  ultrasonic_sensor_handler__get_filtered_sensor_value_right_ExpectAndReturn(sensor2);
  ultrasonic_sensor_handler__get_filtered_sensor_value_front_ExpectAndReturn(sensor3);
  ultrasonic_sensor_handler__get_filtered_sensor_value_back_ExpectAndReturn(sensor4);
  ultrasonic_sensor_handler__set_all_sensor_values_Expect(sensor1, sensor2, sensor3, sensor4);
<<<<<<< HEAD
=======
  led_handler__diagnostic_test_object_detection_leds_for_each_sensor_Expect(sensor1, sensor2, sensor3, sensor4);
>>>>>>> sensor module changes

  can__msg_t sensor_can_msg = {};
  const dbc_message_header_t sensor_header =
      dbc_encode_SENSOR_USONARS(sensor_can_msg.data.bytes, &ultrasonic_sensors_struct);

  sensor_can_msg.msg_id = sensor_header.message_id;
  sensor_can_msg.frame_fields.data_len = sensor_header.message_dlc;

  can__tx_ExpectAnyArgsAndReturn(true);

  sensor_can_handler__transmit_messages_50hz();
}

void test_sensor_can_handler__handle_all_incoming_messages(void) {
  can__msg_t sensor_can_msg = {};
  dbc_SENSOR_USONARS_s decoded_sensor_cmd = {};

  can__rx_ExpectAnyArgsAndReturn(true);
  const dbc_message_header_t header = {
      .message_id = sensor_can_msg.msg_id,
      .message_dlc = sensor_can_msg.frame_fields.data_len,
  };

  dbc_decode_SENSOR_USONARS(&decoded_sensor_cmd, header, sensor_can_msg.data.bytes);

  can__rx_ExpectAnyArgsAndReturn(false);

  sensor_can_handler__handle_all_incoming_messages();
}