// #include "unity.h"

// #include <stdbool.h>

// // Mock header file
// #include "Mockboard_io.h"
// #include "Mockcan_bus.h"
// #include "Mockgpio.h"
// #include "Mocksjvalley_lcd.h"

// #include "Mockproject_debug.h"
// #include "can_bus_mia_config.c"
// #include "driver_diagnostics.h"
// #include "driver_logic.c"
// #include "driving_algo.c"
// #include "full_can_bus_handler.c"
// #include "obstacle_avoidance.c"

// const uint16_t test_CAN_BAUD_RATE = 100;
// const uint16_t test_CAN_TRANSMIT_QUEUE_SIZE = 100;
// const uint16_t test_CAN_RECEIVE_QUEUE_SIZE = 100;
// const can__num_e test_CAN_BUS = can1;

// gpio_s board_led_0 = {GPIO__PORT_2, 3};
// gpio_s board_led_1 = {GPIO__PORT_1, 26};
// gpio_s board_led_2 = {GPIO__PORT_1, 24};
// gpio_s board_led_3 = {GPIO__PORT_1, 18};

// dbc_CAR_ACTION_s test_can_car_action = {};

// bool sjvalley_lcd__send_line_hijacked_stub(uint8_t line_number, char *line, int callback_count) {
//   PROJECT_DEBUG__PRINTF("Line = %d, MSG = %s\n", line_number, line);
//   return true;
// }

// void test_full_can_bus_handler__init(void) {
//   TEST_IGNORE_MESSAGE("Test to be implemented\n");
//   gpio__set_Expect(board_led_0);
//   gpio__set_Expect(board_led_1);
//   gpio__set_Expect(board_led_2);
//   gpio__set_Expect(board_led_3);
//   board_io__get_led0_ExpectAndReturn(board_led_0);
//   board_io__get_led1_ExpectAndReturn(board_led_1);
//   board_io__get_led2_ExpectAndReturn(board_led_2);
//   board_io__get_led3_ExpectAndReturn(board_led_3);
//   can__init_ExpectAndReturn(test_CAN_BUS, test_CAN_BAUD_RATE, test_CAN_RECEIVE_QUEUE_SIZE,
//   test_CAN_TRANSMIT_QUEUE_SIZE,
//                             NULL, NULL, true);
//   // can__bypass_filter_accept_all_msgs_Expect();
//   can__setup_filter_IgnoreAndReturn(true);
//   can_std_id_t ret;
//   can_ext_id_t ext_ret;
//   can__generate_standard_id_IgnoreAndReturn(ret);
//   can__generate_extended_id_IgnoreAndReturn(ext_ret);
//   can__fullcan_add_entry_IgnoreAndReturn(true);
//   can__reset_bus_Expect(test_CAN_BUS);
//   full_can_bus_handler__init();
// }

// void test_can_dbc_handler__reset_if_bus_off(void) {
//   TEST_IGNORE_MESSAGE("Test to be implemented\n");
//   can__is_bus_off_ExpectAndReturn(test_CAN_BUS, true);
//   can__reset_bus_Expect(test_CAN_BUS);
//   gpio__reset_Expect(board_led_0);
//   sjvalley_lcd__send_line_StubWithCallback(sjvalley_lcd__send_line_hijacked_stub);
//   full_can_bus_handler__reset_if_bus_off();

//   can__is_bus_off_ExpectAndReturn(test_CAN_BUS, false);
//   gpio__set_Expect(board_led_0);
//   sjvalley_lcd__send_line_StubWithCallback(sjvalley_lcd__send_line_hijacked_stub);
//   full_can_bus_handler__reset_if_bus_off();
// }

// bool can__rx_ultrasonic_hijacked_mock(can__num_e can, can__msg_t *msg_ptr, uint32_t timeout_ms, int callback_count) {
//   bool return_flag = false;
//   if (callback_count == 0) {
//     msg_ptr->frame_fields.data_len = 5;
//     msg_ptr->msg_id = 200;
//     msg_ptr->data.bytes[0] = 4;
//     msg_ptr->data.bytes[1] = ((uint8_t)3 << 2);
//     msg_ptr->data.bytes[2] = ((uint8_t)2 << 4);
//     msg_ptr->data.bytes[3] = ((uint8_t)1 << 6);
//     return_flag = true;
//   }
//   return return_flag;
// }

// bool can__rx_car_start_stop_hijacked_stub(can__num_e can, can__msg_t *msg_ptr, uint32_t timeout_ms,
//                                           int callback_count) {
//   bool return_flag = false;
//   msg_ptr->frame_fields.data_len = 1;
//   msg_ptr->msg_id = 101;
//   if (callback_count == 0) {
//     msg_ptr->data.bytes[0] = 0;
//     return_flag = true;
//   } else if (callback_count == 1) {
//     msg_ptr->data.bytes[0] = 1;
//     return_flag = true;
//   } else if (callback_count == 2) {
//     msg_ptr->data.bytes[0] = 0;
//     return_flag = true;
//   }
//   return return_flag;
// }

// bool can__rx_compass_data_hijacked_mock(can__num_e can, can__msg_t *msg_ptr, uint32_t timeout_ms, int callback_count)
// {
//   bool return_flag = false;
//   if (callback_count == 0) {
//     msg_ptr->frame_fields.data_len = 6;
//     msg_ptr->msg_id = 401;
//     uint16_t current_heading_angle = 25991;
//     uint16_t dest_heading_angle = 12333;
//     uint16_t distance = 14567;
//     msg_ptr->data.bytes[0] = (current_heading_angle & 0xff);
//     msg_ptr->data.bytes[1] = (current_heading_angle & 0xff00) >> 8;
//     msg_ptr->data.bytes[2] = (dest_heading_angle & 0xff);
//     msg_ptr->data.bytes[3] = (dest_heading_angle & 0xff00) >> 8;
//     msg_ptr->data.bytes[4] = (distance & 0xff);
//     msg_ptr->data.bytes[5] = (distance & 0xff00) >> 8;
//     return_flag = true;
//   }
//   return return_flag;
// }

// void test_full_can_bus_handler__process_all_received_messages_ultrasonic_sensor(void) {
//   TEST_IGNORE_MESSAGE("Test to be implemented\n");
//   can__fullcan_read_msg_copy_StubWithCallback(can__rx_ultrasonic_hijacked_mock);
//   gpio__set_Expect(board_led_1);
//   sjvalley_lcd__send_line_StubWithCallback(sjvalley_lcd__send_line_hijacked_stub);
//   full_can_bus_handler__receive_messages();
//   TEST_ASSERT_EQUAL_UINT16(4, sensor_data.SENSOR_USONARS_left);
//   TEST_ASSERT_EQUAL_UINT16(3, sensor_data.SENSOR_USONARS_right);
//   TEST_ASSERT_EQUAL_UINT16(2, sensor_data.SENSOR_USONARS_front);
//   TEST_ASSERT_EQUAL_UINT16(1, sensor_data.SENSOR_USONARS_back);
// }

// void test_can_bus_handler__process_all_received_messages_geo_compass_data(void) {
//   TEST_IGNORE_MESSAGE("Test to be implemented\n");
//   can__fullcan_read_msg_copy_StubWithCallback(can__rx_compass_data_hijacked_mock);
//   sjvalley_lcd__send_line_StubWithCallback(sjvalley_lcd__send_line_hijacked_stub);
//   can_bus_handler__process_all_received_messages_in_50hz();
//   TEST_ASSERT_EQUAL_FLOAT(259.91, current_and_destination_heading_angle.GEO_COMPASS_current_heading);
//   TEST_ASSERT_EQUAL_FLOAT(123.33, current_and_destination_heading_angle.GEO_COMPASS_desitination_heading);
//   TEST_ASSERT_EQUAL_FLOAT(145.67, current_and_destination_heading_angle.GEO_COMPASS_distance);
// }

// void test_can_bus_handler__process_all_received_messages_car_start_stop_command(void) {
//   TEST_IGNORE_MESSAGE("Test to be implemented\n");
//   can__msg_t msg_data = {};
//   msg_data.msg_id = 101;
//   msg_data.data.bytes[0] = 0;
//   can__rx_ExpectAndReturn(test_CAN_BUS, NULL, 0, true);
//   can__rx_IgnoreArg_can_message_ptr();
//   can__rx_ReturnThruPtr_can_message_ptr(&msg_data);
//   can__rx_ExpectAndReturn(test_CAN_BUS, NULL, 0, false);
//   can__rx_IgnoreArg_can_message_ptr();
//   can__rx_ReturnThruPtr_can_message_ptr(&msg_data);

//   // sjvalley_lcd__send_line_StubWithCallback(sjvalley_lcd__send_line_hijacked_stub);
//   can_bus_handler__process_all_received_messages_in_50hz();
//   TEST_ASSERT_EQUAL_UINT8(0, CAR_IN_START_MODE);

//   can__rx_ExpectAndReturn(test_CAN_BUS, NULL, 0, true);
//   can__rx_IgnoreArg_can_message_ptr();
//   can__rx_ReturnThruPtr_can_message_ptr(&msg_data);
//   msg_data.data.bytes[0] = 1;
//   can__rx_ExpectAndReturn(test_CAN_BUS, NULL, 0, false);
//   can__rx_IgnoreArg_can_message_ptr();
//   can__rx_ReturnThruPtr_can_message_ptr(&msg_data);

//   can_bus_handler__process_all_received_messages_in_50hz();
//   TEST_ASSERT_EQUAL_UINT8(1, CAR_IN_START_MODE);

//   // can_bus_handler__process_all_received_messages_in_100hz();
//   // TEST_ASSERT_EQUAL_UINT8(0, can_car_action.CAR_ACTION_cmd);
// }

// void test_can_bus_handler__transmit_message(void) {
//   TEST_IGNORE_MESSAGE("Test to be implemented\n");
//   can__msg_t send_msg = {};
//   can__tx_ExpectAndReturn(test_CAN_BUS, &send_msg, 0, true);
//   can__tx_IgnoreArg_can_message_ptr();
//   gpio__set_Ignore();
//   gpio__reset_Ignore();
//   sjvalley_lcd__send_line_StubWithCallback(sjvalley_lcd__send_line_hijacked_stub);
//   can_bus_handler__transmit_message_in_20hz();
// }

// void test_can_bus_handler__manage_mia_50hz(void) {
//   TEST_IGNORE_MESSAGE("Test to be implemented\n");
//   const uint32_t mia_increment_value = 100;
//   gpio__reset_Expect(board_led_1);
//   can_sensor_data.mia_info.mia_counter = dbc_mia_threshold_SENSOR_USONARS - mia_increment_value;
//   sjvalley_lcd__send_line_StubWithCallback(sjvalley_lcd__send_line_hijacked_stub);

//   can_bus_handler__manage_mia_50hz();
// }