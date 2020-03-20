#include "can_bus_handler.h"

// Standard Header Files
#include <stdio.h>
#include <string.h>

// User-Defined Header Files
#include "can_bus.h"
#include "driver_logic.h"

// Constants for CAN BUS
static const uint16_t CAN_BAUD_RATE = 100;
static const uint16_t CAN_TRANSMIT_QUEUE_SIZE = 100;
static const uint16_t CAN_RECEIVE_QUEUE_SIZE = 100;
static const can__num_e CAN_BUS = can1;

static dbc_SENSOR_USONARS_s sensor_data = {};

static void can_bus_handler__board_led_reset(void) {
  gpio__set(board_io__get_led0());
  gpio__set(board_io__get_led1());
  gpio__set(board_io__get_led2());
  gpio__set(board_io__get_led3());
}

void can_bus_handler__init(void) {
  can__init(CAN_BUS, CAN_BAUD_RATE, CAN_RECEIVE_QUEUE_SIZE, CAN_TRANSMIT_QUEUE_SIZE, NULL, NULL);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(CAN_BUS);
  can_bus_handler__board_led_reset();
}

void can_bus_handler__reset_if_bus_off(void) {
  if (can__is_bus_off(CAN_BUS)) {
    can__reset_bus(CAN_BUS);
  }
}

void can_bus_handler__process_all_received_messages_in_10hz(void) {
  can__msg_t can_receive_msg = {};
  while (can__rx(CAN_BUS, &can_receive_msg, 0)) {
    const dbc_message_header_t header = {.message_id = can_receive_msg.msg_id,
                                         .message_dlc = can_receive_msg.frame_fields.data_len};
    if (dbc_decode_SENSOR_USONARS(&sensor_data, header, can_receive_msg.data.bytes)) {
      driver_logic__process_ultrasonic_sensors_data(sensor_data);
      gpio__set(board_io__get_led0());
    }
  }
}

void can_bus_handler__transmit_message_in_10hz(void) {
  // receive data from queue and then encode and transmit
  dbc_DRIVER_STEER_SPEED_s steer_info = {};
  can__msg_t can_transmit_msg = {};
  steer_info = driver_logic__get_motor_command();
  dbc_encode_and_send_DRIVER_STEER_SPEED(&can_transmit_msg, &steer_info);
}

void can_bus_handler__manage_mia_10hz(void) {
  const uint32_t mia_increment_value = 100;
  if (dbc_service_mia_SENSOR_USONARS(&sensor_data, mia_increment_value)) {
    driver_logic__process_ultrasonic_sensors_data(sensor_data);
    gpio__reset(board_io__get_led0());
  }
}

bool dbc_send_can_message(void *argument, uint32_t message_id, const uint8_t bytes[8], uint8_t dlc) {
  can__msg_t send_msg = {};
  send_msg.msg_id = message_id;
  send_msg.frame_fields.data_len = dlc;
  memcpy(send_msg.data.bytes, bytes, dlc);
  return can__tx(CAN_BUS, &send_msg, 0);
}