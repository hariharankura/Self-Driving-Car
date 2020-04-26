#include "can_bus.h"
#include "project.h"
#include <stdio.h>

static const uint16_t CAN_BAUD_RATE = 100;
static const uint16_t CAN_TRANSMIT_QUEUE_SIZE = 100;
static const uint16_t CAN_RECEIVE_QUEUE_SIZE = 100;
static const can__num_e CAN_BUS = can1;
static uint32_t id = 101;

void full_can_bus_handler__init(void) {
  can__init(CAN_BUS, CAN_BAUD_RATE, CAN_RECEIVE_QUEUE_SIZE, CAN_TRANSMIT_QUEUE_SIZE, NULL, NULL);
  //   can__bypass_filter_accept_all_msgs();
  const can_std_id_t slist[] = {can__generate_standard_id(can1, 0x100)};
  if (!can__setup_filter(slist, 1, NULL, 0, NULL, 0, NULL, 0)) {
    printf("Setup filter failed\n");
  }
  can__reset_bus(CAN_BUS);

  if (!can__fullcan_add_entry(CAN_BUS, can__generate_standard_id(CAN_BUS, id),
                             can__generate_standard_id(CAN_BUS, id + 1))) {
    printf("Add can entry failed\n");
  }
  if (2 != can__fullcan_get_num_entries()) {
    printf("can entry not added\n");
  }
  //   can_bus_handler__board_led_reset();
}

void full_can_bus_handler__receive_messages(void) {
  can__fullcan_msg_t *car_action = can__fullcan_get_entry_ptr(can__generate_standard_id(CAN_BUS, id));
  can__fullcan_msg_t can_message_temp;

  dbc_CAR_ACTION_s can_car_action = {};
  if (!can__fullcan_read_msg_copy(car_action, &can_message_temp)) {
    printf("message not received\n");
  } else {
    const dbc_message_header_t header = {.message_id = can_message_temp.msg_id,
                                         .message_dlc = can_message_temp.data_len};
    if (dbc_decode_CAR_ACTION(&can_car_action, header, can_message_temp.data.bytes))
      printf("Message  = %d\n", can_car_action.CAR_ACTION_cmd);
  }
}