#include "unity.h"

#include "Mocki2c.h"

#include "Mockgps.h"
#include "project.h"

#include "compass.c"

bool dummy_i2c__read_slave_data(i2c_e i2c_number, uint8_t slave_address, uint8_t starting_slave_memory_address,
                                uint8_t *bytes_to_read, uint32_t number_of_bytes, int callback_count) {
  uint16_t read_data = 2509;
  bytes_to_read[0] = read_data >> 8;
  bytes_to_read[1] = read_data;
  return true;
}
void test_compass_read_current_angle(void) {
  i2c__detect_ExpectAndReturn(I2C__2, 0XC0, true);
  i2c__read_slave_data_StubWithCallback(dummy_i2c__read_slave_data);
  float test_angle = compass__read_current_angle();
  TEST_ASSERT_EQUAL_FLOAT(250.9, test_angle);
}

void test_compass_calculate_destination_angle_1(void) {
  dbc_BRIDGE_GPS_s test_source, test_destination;
  test_source.BRIDGE_GPS_latitude = 55.739399;
  test_source.BRIDGE_GPS_longitude = 37.592572;
  test_destination.BRIDGE_GPS_latitude = 55.735632;
  test_destination.BRIDGE_GPS_longitude = 37.678367;

  float test_destination_angle = (compass__calculate_destination_angle(test_source, test_destination));
  TEST_ASSERT_EQUAL_FLOAT(94.43, test_destination_angle);
}

void test_compass_calculate_destination_angle_2(void) {
  dbc_BRIDGE_GPS_s test_source, test_destination;

  test_source.BRIDGE_GPS_latitude = 39.099912;
  test_source.BRIDGE_GPS_longitude = -94.581213;
  test_destination.BRIDGE_GPS_latitude = 38.627089;
  test_destination.BRIDGE_GPS_longitude = -90.200203;

  float test_destination_angle = (compass__calculate_destination_angle(test_source, test_destination));
  TEST_ASSERT_EQUAL_FLOAT(96.51, test_destination_angle);
}

void test_compass_calculate_destination_angle_3(void) {
  dbc_BRIDGE_GPS_s test_source, test_destination;

  test_source.BRIDGE_GPS_latitude = -43.789077;
  test_source.BRIDGE_GPS_longitude = -45.23459;
  test_destination.BRIDGE_GPS_latitude = -78.23579;
  test_destination.BRIDGE_GPS_longitude = -60.78956;

  float test_destination_angle = (compass__calculate_destination_angle(test_source, test_destination));
  TEST_ASSERT_EQUAL_FLOAT(185.47, test_destination_angle);
}

void test_compass_calculate_destination_angle_4(void) {
  dbc_BRIDGE_GPS_s test_source, test_destination;

  test_source.BRIDGE_GPS_latitude = -23.95732;
  test_source.BRIDGE_GPS_longitude = 34.39489;
  test_destination.BRIDGE_GPS_latitude = -67.83496;
  test_destination.BRIDGE_GPS_longitude = 87.437343;

  float test_destination_angle = (compass__calculate_destination_angle(test_source, test_destination));
  TEST_ASSERT_EQUAL_FLOAT(158.21, test_destination_angle);
}

void test_compass_calculate_destination_angle_5(void) {
  dbc_BRIDGE_GPS_s test_source, test_destination;

  test_source.BRIDGE_GPS_latitude = 90.87215;
  test_source.BRIDGE_GPS_longitude = 91.23790;
  test_destination.BRIDGE_GPS_latitude = 94.18732;
  test_destination.BRIDGE_GPS_longitude = 95.81973;

  float test_destination_angle = (compass__calculate_destination_angle(test_source, test_destination));
  TEST_ASSERT_EQUAL_FLOAT(0, test_destination_angle);
}

void test_convert_source_destination_to_radian_negative_angle(void) {
  dbc_BRIDGE_GPS_s test_source, test_destination;
  test_source.BRIDGE_GPS_latitude = -39.099912;
  test_source.BRIDGE_GPS_longitude = -94.581213;
  test_destination.BRIDGE_GPS_latitude = -38.627089;
  test_destination.BRIDGE_GPS_longitude = -90.200203;
  convert_source_destination_to_radian(&test_source, &test_destination);
  TEST_ASSERT_EQUAL_FLOAT(-0.682422, test_source.BRIDGE_GPS_latitude);
  TEST_ASSERT_EQUAL_FLOAT(1.650753, test_source.BRIDGE_GPS_longitude);
  TEST_ASSERT_EQUAL_FLOAT(-0.674169, test_destination.BRIDGE_GPS_latitude);
  TEST_ASSERT_EQUAL_FLOAT(1.574290, test_destination.BRIDGE_GPS_longitude);
}

void test_convert_source_destination_to_radian_positive_angle(void) {
  dbc_BRIDGE_GPS_s test_source, test_destination;
  test_source.BRIDGE_GPS_latitude = 23.95732;
  test_source.BRIDGE_GPS_longitude = 34.39489;
  test_destination.BRIDGE_GPS_latitude = 67.83496;
  test_destination.BRIDGE_GPS_longitude = 87.437343;
  convert_source_destination_to_radian(&test_source, &test_destination);
  TEST_ASSERT_EQUAL_FLOAT(0.41813411395, test_source.BRIDGE_GPS_latitude);
  TEST_ASSERT_EQUAL_FLOAT(-0.60030407636, test_source.BRIDGE_GPS_longitude);
  TEST_ASSERT_EQUAL_FLOAT(1.1839434, test_destination.BRIDGE_GPS_latitude);
  TEST_ASSERT_EQUAL_FLOAT(-1.52606952454, test_destination.BRIDGE_GPS_longitude);
}

void test_round_upto_2_decimal(void) {
  float test_bearing_1 = 1.5789, test_bearing_2 = 1.5723, value1, value2;
  value1 = round_upto_2_decimal(test_bearing_1);
  value2 = round_upto_2_decimal(test_bearing_1);
  TEST_ASSERT_EQUAL_FLOAT(1.58, value1);
  TEST_ASSERT_EQUAL_FLOAT(1.58, value2);
}

void test_compass__get_current_and_destination_heading(void) {
  i2c__detect_ExpectAndReturn(I2C__2, 0XC0, true);
  i2c__read_slave_data_StubWithCallback(dummy_i2c__read_slave_data);
  current_gps_coordinates.BRIDGE_GPS_latitude = -23.95732;
  current_gps_coordinates.BRIDGE_GPS_longitude = 34.39489;
  destination_gps_coordinates.BRIDGE_GPS_latitude = -67.83496;
  destination_gps_coordinates.BRIDGE_GPS_longitude = 87.437343;
  dbc_GEO_COMPASS_s test_compass_data;
  test_compass_data = compass__get_current_and_destination_heading();
  TEST_ASSERT_EQUAL_FLOAT(158.21, test_compass_data.GEO_COMPASS_desitination_heading);
  TEST_ASSERT_EQUAL_FLOAT(250.9, test_compass_data.GEO_COMPASS_current_heading);
}