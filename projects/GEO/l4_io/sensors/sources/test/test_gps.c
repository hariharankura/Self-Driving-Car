#include "unity.h"

#include <stdio.h>
// Mocks
#include "Mockclock.h"
#include "Mockgpio.h"
#include "Mockuart.h"

#include "Mockboard_io.h"
#include "Mockqueue.h"
// Use the real implementation (not mocks) for:
#include "line_buffer.h"

#include "sl_string.h"

// Include the source we wish to test
#include "gps.c"

void __gps_init(void);

void setUp(void) { __gps_init(); }
// void setUp(void) {}
void tearDown(void) {}

void test_gps__init(void) { __gps_init(); }
void test_gps_other_stuff(void) {}
gpio_s test_gpio_op = {1, 18};
void test_gps__parse_nema_string(void) {

  gps_coordinates_t temp_coordinates;
  char line_N_W[] = "$GPGGA,230612.015,667.0013,N,12102.4634,W,2,04,5.7,508.3,M,,,,0000*13";
  board_io__get_led3_ExpectAndReturn(test_gpio_op);
  gpio__reset_Expect(test_gpio_op);
  TEST_ASSERT_TRUE(gps__parse_nema_string(line_N_W, &temp_coordinates));
  TEST_ASSERT_EQUAL_FLOAT(7.116688, temp_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-121.041057, temp_coordinates.longitude);

  char line_S_W[] = "$GPGGA,230612.015,667.0013,S,12102.4634,W,2,04,5.7,508.3,M,,,,0000*13";
  board_io__get_led3_ExpectAndReturn(test_gpio_op);
  gpio__reset_Expect(test_gpio_op);
  TEST_ASSERT_TRUE(gps__parse_nema_string(line_S_W, &temp_coordinates));
  TEST_ASSERT_EQUAL_FLOAT(-7.116688, temp_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-121.041057, temp_coordinates.longitude);

  char line_N_E[] = "$GPGGA,230612.015,667.0013,N,12102.4634,E,2,04,5.7,508.3,M,,,,0000*13";
  board_io__get_led3_ExpectAndReturn(test_gpio_op);
  gpio__reset_Expect(test_gpio_op);
  TEST_ASSERT_TRUE(gps__parse_nema_string(line_N_E, &temp_coordinates));
  TEST_ASSERT_EQUAL_FLOAT(7.116688, temp_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(121.041057, temp_coordinates.longitude);

  char line_S_E[] = "$GPGGA,230612.015,667.0013,S,12102.4634,E,2,04,5.7,508.3,M,,,,0000*13";
  board_io__get_led3_ExpectAndReturn(test_gpio_op);
  gpio__reset_Expect(test_gpio_op);
  TEST_ASSERT_TRUE(gps__parse_nema_string(line_S_E, &temp_coordinates));
  TEST_ASSERT_EQUAL_FLOAT(-7.116688, temp_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(121.041057, temp_coordinates.longitude);
}

void test_gps__parse_nema_string_temp(void) {
  gps_coordinates_t temp_coordinates;
  char line_N_W[] = "$GPGGA,230612.015,4.9310,S,0.8825,W,0,04,5.7,508.3,M,,,,0000*13";
  board_io__get_led3_ExpectAndReturn(test_gpio_op);
  gpio__reset_Expect(test_gpio_op);
  TEST_ASSERT_FALSE(gps__parse_nema_string(line_N_W, &temp_coordinates));
  TEST_ASSERT_EQUAL_FLOAT(-0.082183, temp_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-0.01470833, temp_coordinates.longitude);
}

void test_gps__parse_nema_string_invalid_data(void) {
  gps_coordinates_t temp_coordinates;
  char line[] = "$GPGGA,230612.015,667.0013,N,12102.4634,W,0,04,5.7,508.3,M,,,,0000*13";
  board_io__get_led3_ExpectAndReturn(test_gpio_op);
  gpio__reset_Expect(test_gpio_op);
  TEST_ASSERT_FALSE(gps__parse_nema_string(line, &temp_coordinates));
  TEST_ASSERT_EQUAL_FLOAT(7.116688, temp_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(-121.041057, temp_coordinates.longitude);
}

void test_gps__parse_invalid_string(void) {
  gps_coordinates_t temp_coordinates = {0};
  char line[] = ",123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
  TEST_ASSERT_FALSE(gps__parse_nema_string(line, &temp_coordinates));
  TEST_ASSERT_EQUAL_FLOAT(0, temp_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(0, temp_coordinates.longitude);
}

bool uart__get_stub(uart_e uart, char *input_byte, uint32_t timeout_ms, int callback_count) {
  static int iterate = 0;
  static char test_string[] = "$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n$GPGGA,123519,4808.038,N,01132.000,E,"
                              "1,08,0.9,545.4,M,46.9,M,,*47\n$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n";
  if (iterate < sizeof(test_string)) {
    *input_byte = test_string[iterate++];
    return true;
  } else {
    return false;
  }
}

bool uart__get_incorrect_stub(uart_e uart, char *input_byte, uint32_t timeout_ms, int callback_count) {
  static int iterate = 0;
  static char test_string[] =
      "$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\nGPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n";
  if (iterate < sizeof(test_string)) {
    *input_byte = test_string[iterate++];
    return true;
  } else {
    return false;
  }
}

void test_gps_handle_line(void) {
  static const char gps_string[] = "$GPGGA,123519,4808.038,N,01132.000,E,1,08,0.9,545.4,M,46.9,M,,*47\n";
  for (int i = 0; i < sizeof(gps_string); i++) {
    line_buffer__add_byte(&line, gps_string[i]);
  }
  board_io__get_led3_ExpectAndReturn(test_gpio_op);
  gpio__set_Expect(test_gpio_op);
  gps__handle_line();
  TEST_ASSERT_EQUAL_FLOAT(48.133967, parsed_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(11.533333, parsed_coordinates.longitude);
}

void test_gps__get_data(void) {
  char byte;
  uart__get_StubWithCallback(uart__get_stub);
  board_io__get_led3_ExpectAndReturn(test_gpio_op);
  gpio__set_Expect(test_gpio_op);
  gps__absorb_data();
  gps__handle_line();
  gps__handle_line();
  gps_coordinates_t temp_coordinates;
  temp_coordinates = gps__get_coordinates();
  TEST_ASSERT_EQUAL_FLOAT(48.133967, temp_coordinates.latitude);
  TEST_ASSERT_EQUAL_FLOAT(11.533333, temp_coordinates.longitude);
}

void __gps_init(void) {
  gpio_s rx, tx;
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 28, GPIO__FUNCTION_2, rx);
  gpio__construct_with_function_ExpectAndReturn(GPIO__PORT_4, 29, GPIO__FUNCTION_2, tx);
  clock__get_peripheral_clock_hz_ExpectAndReturn(96 * 1000 * 1000);
  uart__init_Expect(gps_uart, 96 * 1000 * 1000, 9600);
  QueueHandle_t rxq_handle;
  QueueHandle_t txq_handle;
  xQueueCreate_ExpectAndReturn(200, sizeof(char), rxq_handle);
  xQueueCreate_ExpectAndReturn(8, sizeof(char), txq_handle);
  uart__enable_queues_ExpectAndReturn(gps_uart, rxq_handle, txq_handle, true);
  gps__init();
}
