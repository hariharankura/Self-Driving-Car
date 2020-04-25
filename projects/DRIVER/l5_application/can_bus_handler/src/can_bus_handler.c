#include "can_bus_handler.h"

// Standard Header Files
#include <stdio.h>
#include <string.h>

// User-Defined Header Files
#include "board_io.h"
#include "gpio.h"

#include "can_bus.h"
#include "driver_logic.h"
#include "driving_algo.h"
#include "obstacle_avoidance.h"
#include "project_debug.h"

// Constants for CAN BUS
static const uint16_t CAN_BAUD_RATE = 100;
static const uint16_t CAN_TRANSMIT_QUEUE_SIZE = 100;
static const uint16_t CAN_RECEIVE_QUEUE_SIZE = 100;
static const can__num_e CAN_BUS = can1;

static gpio_s CAN_BUS_OFF_STATUS_LED;
static gpio_s MIA_OBSTACLE_STATUS_LED;

static dbc_SENSOR_USONARS_s can_sensor_data = {};
static dbc_GEO_COMPASS_s can_current_and_destination_heading_angle = {};
static dbc_CAR_ACTION_s can_car_action = {};

static void can_bus_handler__board_led_reset(void) {
  CAN_BUS_OFF_STATUS_LED = board_io__get_led0();
  MIA_OBSTACLE_STATUS_LED = board_io__get_led1();
  gpio__set(CAN_BUS_OFF_STATUS_LED);
  gpio__set(MIA_OBSTACLE_STATUS_LED);
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
    gpio__reset(CAN_BUS_OFF_STATUS_LED);
    printf("Buss off\n");
    PROJECT_DEBUG__LCD_PRINTF(0, "CAN BUS OFF");
  } else {
    gpio__set(CAN_BUS_OFF_STATUS_LED);
    PROJECT_DEBUG__LCD_PRINTF(0, "CAN BUS ON");
  }
}

void can_bus_handler__process_all_received_messages_in_50hz(void) {
  can__msg_t can_receive_msg = {};
  PROJECT_DEBUG__PRINTF("TRYING TO RECEIVE\n");
  while (can__rx(CAN_BUS, &can_receive_msg, 0)) {
    const dbc_message_header_t header = {.message_id = can_receive_msg.msg_id,
                                         .message_dlc = can_receive_msg.frame_fields.data_len};
    if (dbc_decode_SENSOR_USONARS(&can_sensor_data, header, can_receive_msg.data.bytes)) {
      obstacle_avoidance__process_ultrasonic_sensors_data(can_sensor_data);
      PROJECT_DEBUG__PRINTF("Recieved OBS\n");
      obstacle_avoidance__print_debug_data();
      gpio__set(MIA_OBSTACLE_STATUS_LED);
    } else if (dbc_decode_GEO_COMPASS(&can_current_and_destination_heading_angle, header, can_receive_msg.data.bytes)) {
      printf("Received Data\n");
      driving_algo__process_geo_compass_data(can_current_and_destination_heading_angle);
    } else if (dbc_decode_CAR_ACTION(&can_car_action, header, can_receive_msg.data.bytes)) {
      driver_logic__set_car_mode(can_car_action);
    }
  }
}

void can_bus_handler__transmit_message_in_20hz(void) {
  dbc_DRIVER_STEER_SPEED_s steer_info = {};
  can__msg_t can_transmit_msg = {};
  steer_info = driver_logic__get_motor_command();
  // PROJECT_DEBUG__LCD_PRINTF(2, "s=%d,d=%d", steer_info.DRIVER_STEER_direction, steer_info.DRIVER_STEER_move_speed);
  // printf("s=%d,d=%d\n", steer_info.DRIVER_STEER_direction, steer_info.DRIVER_STEER_move_speed);
  dbc_encode_and_send_DRIVER_STEER_SPEED(&can_transmit_msg, &steer_info);
}

static void can_bus_handler__mia_for_usonars() {
  obstacle_avoidance__process_ultrasonic_sensors_data(can_sensor_data);
  gpio__reset(MIA_OBSTACLE_STATUS_LED);
  PROJECT_DEBUG__LCD_PRINTF(1, "Obs MIA");
  PROJECT_DEBUG__PRINTF("Obstacle MIA\n");
}

void can_bus_handler__manage_mia_50hz(void) {
  const uint32_t mia_increment_value = 100;
  if (dbc_service_mia_SENSOR_USONARS(&can_sensor_data, mia_increment_value)) {
    can_bus_handler__mia_for_usonars();
  }
}

bool dbc_send_can_message(void *argument, uint32_t message_id, const uint8_t bytes[8], uint8_t dlc) {
  can__msg_t send_msg = {};
  send_msg.msg_id = message_id;
  send_msg.frame_fields.data_len = dlc;
  memcpy(send_msg.data.bytes, bytes, dlc);
  return can__tx(CAN_BUS, &send_msg, 0);
}