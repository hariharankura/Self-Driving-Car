#include "can_bus_handler.h"

// Standard Header Files
#include <stdio.h>

// User-Defined Header Files
#include "can_bus.h"
#include "project.h"

// Constants for CAN BUS
const uint16_t CAN_BAUD_RATE = 100;
const uint16_t CAN_TRANSMIT_QUEUE_SIZE = 100;
const uint16_t CAN_RECEIVE_QUEUE_SIZE = 100;
const can__num_e CAN_BUS = can1;

void can_bus_handler__init(void) {
  can__init(CAN_BUS, CAN_BAUD_RATE, CAN_RECEIVE_QUEUE_SIZE, CAN_TRANSMIT_QUEUE_SIZE, NULL, NULL);
  can__bypass_filter_accept_all_msgs();
  can__reset_bus(CAN_BUS);
}

void can_bus_handler__process_all_received_messages(void) {
  can__msg_t can_receive_msg = {};
  while (can__rx(CAN_BUS, &can_receive_msg, 0)) {
    const dbc_message_header_t header = {.message_id = can_receive_msg.msg_id,
                                         .message_dlc = can_receive_msg.frame_fields.data_len};
    dbc_SENSOR_USONARS_s sensor_data = {};
    if (dbc_decode_SENSOR_USONARS(&sensor_data, header, can_receive_msg.data.bytes)) {
      driver_logic__process_ultrasonic_sensors_data(sensor_data);
    }
  }
}

void can_bus_handler__transmit_message(void) {
  // receive data from queue and then encode and transmit
}