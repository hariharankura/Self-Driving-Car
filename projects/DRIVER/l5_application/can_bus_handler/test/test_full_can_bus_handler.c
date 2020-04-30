#include "unity.h"

#include <stdbool.h>

// Mock header file
#include "Mockboard_io.h"
#include "Mockcan_bus.h"
#include "Mockgpio.h"
#include "Mocksjvalley_lcd.h"

#include "Mockproject_debug.h"
#include "can_bus_mia_config.c"
#include "driver_diagnostics.h"
#include "driver_logic.c"
#include "driving_algo.c"
#include "full_can_bus_handler.c"
#include "obstacle_avoidance.c"

const uint16_t test_CAN_BAUD_RATE = 100;
const uint16_t test_CAN_TRANSMIT_QUEUE_SIZE = 100;
const uint16_t test_CAN_RECEIVE_QUEUE_SIZE = 100;
const can__num_e test_CAN_BUS = can1;

gpio_s board_led_0 = {GPIO__PORT_2, 3};
gpio_s board_led_1 = {GPIO__PORT_1, 26};
gpio_s board_led_2 = {GPIO__PORT_1, 24};
gpio_s board_led_3 = {GPIO__PORT_1, 18};

dbc_CAR_ACTION_s test_can_car_action = {};

bool sjvalley_lcd__send_line_hijacked_stub(uint8_t line_number, char *line, int callback_count) {
  PROJECT_DEBUG__PRINTF("Line = %d, MSG = %s\n", line_number, line);
  return true;
}

void test_full_can_bus_handler__init(void) {
  gpio__set_Expect(board_led_0);
  gpio__set_Expect(board_led_1);
  gpio__set_Expect(board_led_2);
  gpio__set_Expect(board_led_3);
  board_io__get_led0_ExpectAndReturn(board_led_0);
  board_io__get_led1_ExpectAndReturn(board_led_1);
  board_io__get_led2_ExpectAndReturn(board_led_2);
  board_io__get_led3_ExpectAndReturn(board_led_3);
  can__init_ExpectAndReturn(test_CAN_BUS, test_CAN_BAUD_RATE, test_CAN_RECEIVE_QUEUE_SIZE, test_CAN_TRANSMIT_QUEUE_SIZE,
                            NULL, NULL, true);
  can_std_id_t ret1, ret2, ret3, ret4;
  can_ext_id_t ext_ret;
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 101, ret1);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 200, ret2);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 401, ret3);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 600, ret4);
  can__fullcan_add_entry_ExpectAndReturn(test_CAN_BUS, ret1, ret2, true);
  can__fullcan_add_entry_ExpectAndReturn(test_CAN_BUS, ret3, ret4, true);
  can__fullcan_get_num_entries_ExpectAndReturn(4);
  can__reset_bus_Expect(test_CAN_BUS);
  full_can_bus_handler__init();
}

void test_can_dbc_handler__reset_if_bus_off(void) {
  can__is_bus_off_ExpectAndReturn(test_CAN_BUS, true);
  can__reset_bus_Expect(test_CAN_BUS);
  gpio__reset_Expect(board_led_0);
  sjvalley_lcd__send_line_StubWithCallback(sjvalley_lcd__send_line_hijacked_stub);
  full_can_bus_handler__reset_if_bus_off();

  can__is_bus_off_ExpectAndReturn(test_CAN_BUS, false);
  gpio__set_Expect(board_led_0);
  sjvalley_lcd__send_line_StubWithCallback(sjvalley_lcd__send_line_hijacked_stub);
  full_can_bus_handler__reset_if_bus_off();
}

void test_can_bus_handler__process_all_received_messages_speed_pwm(void) {
  can__fullcan_msg_t motor_can_msg = {};
  motor_can_msg.msg_id = 600;
  motor_can_msg.data_len = 4;
  uint16_t encode_offset = 3001;
  uint16_t speed = 2056 + encode_offset; // Adding offset for comoensation in encode function
  uint16_t pwm = 1198;
  motor_can_msg.data.bytes[0] = (speed & 0xff);
  motor_can_msg.data.bytes[1] = (speed & 0xff00) >> 8;
  motor_can_msg.data.bytes[2] = (pwm & 0xff);
  motor_can_msg.data.bytes[3] = (pwm & 0xff00) >> 8;
  can_std_id_t ret1, ret2, ret3, ret4;
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 101, ret1);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 200, ret2);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 401, ret3);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 600, ret4);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret1, NULL);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret2, NULL);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret3, NULL);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret4, &motor_can_msg);

  can__fullcan_msg_t can_receive_msg = {};

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  can__fullcan_read_msg_copy_ExpectAndReturn(&motor_can_msg, &motor_can_msg, true);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();
  can__fullcan_read_msg_copy_ReturnThruPtr_msg_copy_ptr(&motor_can_msg);

  full_can_bus_handler__receive_messages();
  TEST_ASSERT_EQUAL_FLOAT(20.56, can_motor_speed.MOTOR_SPEED_info);
  TEST_ASSERT_EQUAL_FLOAT(11.98, can_motor_speed.MOTOR_SPEED_pwm);
}

void test_can_bus_handler__process_all_received_messages_ultrasonic_data(void) {
  can__fullcan_msg_t sonar_can_msg = {};
  sonar_can_msg.msg_id = 200;
  sonar_can_msg.data_len = 5;
  sonar_can_msg.data.bytes[0] = 4;
  sonar_can_msg.data.bytes[1] = ((uint8_t)3 << 2);
  sonar_can_msg.data.bytes[2] = ((uint8_t)2 << 4);
  sonar_can_msg.data.bytes[3] = ((uint8_t)1 << 6);
  can_std_id_t ret1, ret2, ret3, ret4;
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 101, ret1);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 200, ret2);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 401, ret3);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 600, ret4);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret1, NULL);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret2, &sonar_can_msg);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret3, NULL);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret4, NULL);

  can__fullcan_msg_t can_receive_msg = {};

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  can__fullcan_read_msg_copy_ExpectAndReturn(&sonar_can_msg, &sonar_can_msg, true);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();
  can__fullcan_read_msg_copy_ReturnThruPtr_msg_copy_ptr(&sonar_can_msg);

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  gpio__set_Expect(board_led_1);

  full_can_bus_handler__receive_messages();
  TEST_ASSERT_EQUAL_UINT16(4, sensor_data.SENSOR_USONARS_left);
  TEST_ASSERT_EQUAL_UINT16(3, sensor_data.SENSOR_USONARS_right);
  TEST_ASSERT_EQUAL_UINT16(2, sensor_data.SENSOR_USONARS_front);
  TEST_ASSERT_EQUAL_UINT16(1, sensor_data.SENSOR_USONARS_back);
}

void test_can_bus_handler__process_all_received_messages_geo_data(void) {
  can__fullcan_msg_t geo_can_msg = {};
  geo_can_msg.msg_id = 401;
  geo_can_msg.data.bytes[0] = 1;
  geo_can_msg.data_len = 6;
  uint16_t current_heading_angle = 25991;
  uint16_t dest_heading_angle = 12333;
  uint16_t distance = 14567;
  geo_can_msg.data.bytes[0] = (current_heading_angle & 0xff);
  geo_can_msg.data.bytes[1] = (current_heading_angle & 0xff00) >> 8;
  geo_can_msg.data.bytes[2] = (dest_heading_angle & 0xff);
  geo_can_msg.data.bytes[3] = (dest_heading_angle & 0xff00) >> 8;
  geo_can_msg.data.bytes[4] = (distance & 0xff);
  geo_can_msg.data.bytes[5] = (distance & 0xff00) >> 8;
  can_std_id_t ret1, ret2, ret3, ret4;
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 101, ret1);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 200, ret2);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 401, ret3);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 600, ret4);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret1, NULL);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret2, NULL);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret3, &geo_can_msg);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret4, NULL);

  can__fullcan_msg_t can_receive_msg = {};

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  can__fullcan_read_msg_copy_ExpectAndReturn(&geo_can_msg, &geo_can_msg, true);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();
  can__fullcan_read_msg_copy_ReturnThruPtr_msg_copy_ptr(&geo_can_msg);

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  full_can_bus_handler__receive_messages();
  TEST_ASSERT_EQUAL_FLOAT(259.91, current_and_destination_heading_angle.GEO_COMPASS_current_heading);
  TEST_ASSERT_EQUAL_FLOAT(123.33, current_and_destination_heading_angle.GEO_COMPASS_desitination_heading);
  TEST_ASSERT_EQUAL_FLOAT(145.67, current_and_destination_heading_angle.GEO_COMPASS_distance);
}

void test_can_bus_handler__process_all_received_messages_car_start_command(void) {
  can__fullcan_msg_t car_action_can_msg = {};
  car_action_can_msg.msg_id = 101;
  car_action_can_msg.data.bytes[0] = 1;
  car_action_can_msg.data_len = 1;
  can_std_id_t ret1, ret2, ret3, ret4;
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 101, ret1);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 200, ret2);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 401, ret3);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 600, ret4);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret1, &car_action_can_msg);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret2, NULL);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret3, NULL);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret4, NULL);

  can__fullcan_msg_t can_receive_msg = {};
  can__fullcan_read_msg_copy_ExpectAndReturn(&car_action_can_msg, &car_action_can_msg, true);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();
  can__fullcan_read_msg_copy_ReturnThruPtr_msg_copy_ptr(&car_action_can_msg);

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  full_can_bus_handler__receive_messages();
  TEST_ASSERT_EQUAL_UINT8(1, CAR_IN_START_MODE);
}

void test_can_bus_handler__process_all_received_messages_car_stop_command(void) {
  can__fullcan_msg_t car_action_can_msg = {};
  car_action_can_msg.msg_id = 101;
  car_action_can_msg.data.bytes[0] = 0;
  car_action_can_msg.data_len = 1;
  can_std_id_t ret1, ret2, ret3, ret4;
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 101, ret1);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 200, ret2);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 401, ret3);
  can__generate_standard_id_ExpectAndReturn(test_CAN_BUS, 600, ret4);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret1, &car_action_can_msg);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret2, NULL);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret3, NULL);
  can__fullcan_get_entry_ptr_ExpectAndReturn(ret4, NULL);

  can__fullcan_msg_t can_receive_msg = {};
  can__fullcan_read_msg_copy_ExpectAndReturn(&car_action_can_msg, &car_action_can_msg, true);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();
  can__fullcan_read_msg_copy_ReturnThruPtr_msg_copy_ptr(&car_action_can_msg);

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  can__fullcan_read_msg_copy_ExpectAndReturn(NULL, NULL, false);
  can__fullcan_read_msg_copy_IgnoreArg_fc_msg_ptr();
  can__fullcan_read_msg_copy_IgnoreArg_msg_copy_ptr();

  full_can_bus_handler__receive_messages();
  TEST_ASSERT_EQUAL_UINT8(0, CAR_IN_START_MODE);
}

void test_can_bus_handler__transmit_message(void) {
  can__msg_t send_msg = {};
  can__tx_ExpectAndReturn(test_CAN_BUS, &send_msg, 0, true);
  can__tx_IgnoreArg_can_message_ptr();
  gpio__set_Ignore();
  gpio__reset_Ignore();
  sjvalley_lcd__send_line_StubWithCallback(sjvalley_lcd__send_line_hijacked_stub);
  full_can_bus_handler__transmit_message_in_20hz();
}

void test_can_bus_handler__manage_mia_50hz(void) {
  const uint32_t mia_increment_value = 100;
  gpio__reset_Expect(board_led_1);
  can_sensor_data.mia_info.mia_counter = dbc_mia_threshold_SENSOR_USONARS - mia_increment_value;
  sjvalley_lcd__send_line_StubWithCallback(sjvalley_lcd__send_line_hijacked_stub);

  full_can_bus_handler__manage_mia_50hz();
}