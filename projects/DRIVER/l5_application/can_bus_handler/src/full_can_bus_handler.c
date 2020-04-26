#include "board_io.h"
#include "can_bus.h"
#include "gpio.h"
#include "project.h"
#include <stdio.h>

#include "driver_logic.h"
#include "driving_algo.h"
#include "obstacle_avoidance.h"
#include "project_debug.h"

static const uint16_t CAN_BAUD_RATE = 100;
static const uint16_t CAN_TRANSMIT_QUEUE_SIZE = 100;
static const uint16_t CAN_RECEIVE_QUEUE_SIZE = 100;
static const can__num_e CAN_BUS = can1;
static const uint8_t DRIVER_EXPECTED_CAN_MESSAGES = 4; // must be even
static uint32_t ids[] = {101, 200, 401, 600};

static gpio_s CAN_BUS_OFF_STATUS_LED;
static gpio_s MIA_OBSTACLE_STATUS_LED;

static dbc_SENSOR_USONARS_s can_sensor_data = {};
static dbc_GEO_COMPASS_s can_current_and_destination_heading_angle = {};
static dbc_CAR_ACTION_s can_car_action = {};
static dbc_MOTOR_SPEED_s can_motor_speed = {};

static void full_can_bus_handler__board_led_reset(void) {
  CAN_BUS_OFF_STATUS_LED = board_io__get_led0();
  MIA_OBSTACLE_STATUS_LED = board_io__get_led1();
  gpio__set(CAN_BUS_OFF_STATUS_LED);
  gpio__set(MIA_OBSTACLE_STATUS_LED);
  gpio__set(board_io__get_led2());
  gpio__set(board_io__get_led3());
}

void full_can_bus_handler__reset_if_bus_off(void) {
  if (can__is_bus_off(CAN_BUS)) {
    can__reset_bus(CAN_BUS);
    gpio__reset(CAN_BUS_OFF_STATUS_LED);
    printf("Bus off\n");
    PROJECT_DEBUG__LCD_PRINTF(0, "CAN BUS OFF");
  } else {
    gpio__set(CAN_BUS_OFF_STATUS_LED);
    PROJECT_DEBUG__LCD_PRINTF(0, "CAN BUS ON");
  }
}

void full_can_bus_handler__add_all_can_entry(void) {
  for (uint8_t i = 0; i < DRIVER_EXPECTED_CAN_MESSAGES; i += 2) {
    if (!can__fullcan_add_entry(CAN_BUS, can__generate_standard_id(CAN_BUS, ids[i]),
                                can__generate_standard_id(CAN_BUS, ids[i + 1]))) {
      printf("Add can entry failed\n");
    }
  }
  if (DRIVER_EXPECTED_CAN_MESSAGES != can__fullcan_get_num_entries()) {
    printf("can entry not added\n");
  }
}

void full_can_bus_handler__init(void) {
  can__init(CAN_BUS, CAN_BAUD_RATE, CAN_RECEIVE_QUEUE_SIZE, CAN_TRANSMIT_QUEUE_SIZE, NULL, NULL);
  can__reset_bus(CAN_BUS);

  full_can_bus_handler__add_all_can_entry();
  full_can_bus_handler__board_led_reset();
}

void full_can_bus_handler__transmit_message_in_20hz(void) {
  dbc_DRIVER_STEER_SPEED_s steer_info = {};
  can__msg_t can_transmit_msg = {};
  steer_info = driver_logic__get_motor_command();
  dbc_encode_and_send_DRIVER_STEER_SPEED(&can_transmit_msg, &steer_info);
}

void full_can_bus_handler__receive_messages(void) {
  can__fullcan_msg_t *car_action = can__fullcan_get_entry_ptr(can__generate_standard_id(CAN_BUS, ids[0]));
  can__fullcan_msg_t *u_sensors = can__fullcan_get_entry_ptr(can__generate_standard_id(CAN_BUS, ids[1]));
  can__fullcan_msg_t *geo = can__fullcan_get_entry_ptr(can__generate_standard_id(CAN_BUS, ids[2]));
  can__fullcan_msg_t *motor_speed = can__fullcan_get_entry_ptr(can__generate_standard_id(CAN_BUS, ids[3]));
  // can__fullcan_msg_t *headlight = can__fullcan_get_entry_ptr(can__generate_standard_id(CAN_BUS, ids[4]));
  can__fullcan_msg_t can_receive_msg;

  if (can__fullcan_read_msg_copy(car_action, &can_receive_msg)) {
    const dbc_message_header_t header = {.message_id = can_receive_msg.msg_id, .message_dlc = can_receive_msg.data_len};
    if (dbc_decode_CAR_ACTION(&can_car_action, header, can_receive_msg.data.bytes)) {
      driver_logic__set_car_mode(can_car_action);
    }
  }

  if (can__fullcan_read_msg_copy(u_sensors, &can_receive_msg)) {
    const dbc_message_header_t header = {.message_id = can_receive_msg.msg_id, .message_dlc = can_receive_msg.data_len};
    if (dbc_decode_SENSOR_USONARS(&can_sensor_data, header, can_receive_msg.data.bytes)) {
      obstacle_avoidance__process_ultrasonic_sensors_data(can_sensor_data);
      obstacle_avoidance__print_debug_data();
      gpio__set(MIA_OBSTACLE_STATUS_LED);
    }
  }

  if (can__fullcan_read_msg_copy(geo, &can_receive_msg)) {
    const dbc_message_header_t header = {.message_id = can_receive_msg.msg_id, .message_dlc = can_receive_msg.data_len};
    if (dbc_decode_GEO_COMPASS(&can_current_and_destination_heading_angle, header, can_receive_msg.data.bytes)) {
      driving_algo__process_geo_compass_data(can_current_and_destination_heading_angle);
    }
  }

  if (can__fullcan_read_msg_copy(motor_speed, &can_receive_msg)) {
    const dbc_message_header_t header = {.message_id = can_receive_msg.msg_id, .message_dlc = can_receive_msg.data_len};
    if (dbc_decode_MOTOR_SPEED(&can_motor_speed, header, can_receive_msg.data.bytes)) {
      driver_logic__set_car_current_speed(can_motor_speed);
    }
  }
}

static void full_can_bus_handler__mia_for_usonars() {
  obstacle_avoidance__process_ultrasonic_sensors_data(can_sensor_data);
  gpio__reset(MIA_OBSTACLE_STATUS_LED);
  PROJECT_DEBUG__LCD_PRINTF(1, "Obs MIA");
  PROJECT_DEBUG__PRINTF("Obstacle MIA\n");
}

void full_can_bus_handler__manage_mia_50hz(void) {
  const uint32_t mia_increment_value = 100;
  if (dbc_service_mia_SENSOR_USONARS(&can_sensor_data, mia_increment_value)) {
    full_can_bus_handler__mia_for_usonars();
  }
}

bool dbc_send_can_message(void *argument, uint32_t message_id, const uint8_t bytes[8], uint8_t dlc) {
  can__msg_t send_msg = {};
  send_msg.msg_id = message_id;
  send_msg.frame_fields.data_len = dlc;
  memcpy(send_msg.data.bytes, bytes, dlc);
  return can__tx(CAN_BUS, &send_msg, 0);
}