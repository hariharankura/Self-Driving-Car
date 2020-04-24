#include "unity.h"

#include "Mockgpio.h"

#include "Mockclock.h"
#include "Mockuart.h"

#include "Mockqueue.h"
#include "Mockultrasonic_sensor_handler.h"

#include "bridge_buffer.h"
#include "bridge_controller_handler.h"

void test_bridge_controller_handler__initialize_bluetooth_module() {

  gpio_s gpio_data;
  clock__get_peripheral_clock_hz_ExpectAndReturn(96 * 1000 * 1000U);
  uart__init_Expect(UART__3, 96 * 1000U * 1000, 9600);
  QueueHandle_t rxq_handle;
  QueueHandle_t txq_handle;
  xQueueCreate_ExpectAndReturn(300, sizeof(unsigned char), rxq_handle);
  xQueueCreate_ExpectAndReturn(300, sizeof(unsigned char), txq_handle);
  uart__enable_queues_ExpectAndReturn(UART__3, rxq_handle, txq_handle, true);

  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 28, GPIO__FUNCTION_2, gpio_data);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 29, GPIO__FUNCTION_2, gpio_data);

  bridge_controller_handler__initialize_bluetooth_module();
}

void test_bridge_controller_handler__convert_gps_data_to_degrees() {
  gps_data data;
  data.latitude = 5403.456;
  data.longitude = 0005.3453;

  bridge_controller_handler__convert_gps_data_to_degrees(&data);
  TEST_ASSERT_EQUAL(54.0576, data.latitude);
  TEST_ASSERT_EQUAL(0.089088, data.longitude);
}

void test_bridge_controller_handler__assign_lat_long_direction() {
  gps_data data;
  data.latitude = 54.0576;
  data.longitude = 0.089088;
  data.latitude_direction = 'N';
  data.longitude_direction = 'W';

  bridge_controller_handler__assign_lat_long_direction(&data);

  TEST_ASSERT_EQUAL_FLOAT(54.0576, data.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-0.089088, data.longitude);
}

void test_bridge_controller_handler__assign_lat_long_direction_two() {
  gps_data data;
  data.latitude = 54.0576;
  data.longitude = 0.089088;
  data.latitude_direction = 'S';
  data.longitude_direction = 'W';

  bridge_controller_handler__assign_lat_long_direction(&data);

  TEST_ASSERT_EQUAL_FLOAT(-54.0576, data.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-0.089088, data.longitude);
}

void test_bridge_controller_handler__buffer_has_gps_message() {
  int message_count = 0;
  front = 0;
  rear = 100;
  unsigned char test_array[] = "$GPGGA,092751.000,5321.6802,N,00630.3371,W,1,8,1.03,61.7,M,55.3,M,,*75\r\n";

  TEST_ASSERT_TRUE(bridge_controller_handler__buffer_has_gps_message(test_array, &message_count))
  TEST_ASSERT_EQUAL(1, message_count);
}

void test_bridge_controller_handler__parse_gps_data_positive() {
  unsigned char test_array[] = "$GPGGA,092751.000,5321.6802,N,00630.3371,W,1,8,1.03,61.7,M,55.3,M,,*75\r\n";
  float latitude;
  float longitude;

  bridge_controller_handler__parse_gps_data(test_array, &latitude, &longitude);
  TEST_ASSERT_EQUAL_FLOAT(53.361337, latitude);
  TEST_ASSERT_EQUAL_FLOAT(-6.505618, longitude);

  unsigned char test_array_two[] = "$GPGGA,092751.000,5321.6802,S,00630.3371,E,1,8,1.03,61.7,M,55.3,M,,*75\r\n";
  bridge_controller_handler__parse_gps_data(test_array_two, &latitude, &longitude);
  TEST_ASSERT_EQUAL_FLOAT(-53.361337, latitude);
  TEST_ASSERT_EQUAL_FLOAT(6.505618, longitude);
}

void test_bridge_controller_handler__parse_gps_data_negative() {
  unsigned char test_array[] = "$XYZ,092751.000,5321.6802,N,00630.3371,W,1,8,1.03,61.7,M,55.3,M,,*75\r\n";
  float latitude = 0.0f;
  float longitude = 0.0f;

  bridge_controller_handler__parse_gps_data(test_array, &latitude, &longitude);
  TEST_ASSERT_EQUAL_FLOAT(0.0, latitude);
  TEST_ASSERT_EQUAL_FLOAT(0.0, longitude);

  unsigned char test_array_two[] = "$XYZ,092751.000,5321.6802,S,00630.3371,E,1,8,1.03,61.7,M,55.3,M,,*75\r\n";
  bridge_controller_handler__parse_gps_data(test_array_two, &latitude, &longitude);
  TEST_ASSERT_EQUAL_FLOAT(0.0, latitude);
  TEST_ASSERT_EQUAL_FLOAT(0.0, longitude);
}
