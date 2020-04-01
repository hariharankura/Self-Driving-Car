#include "compass.h"
#include "gps.h"
#include "i2c.h"
#include "math.h"
#include "project.h"

static const uint8_t COMPASS_ADDRESS = 0xC0;
static const uint8_t SLAVE_READ_ANGLE = 0x02;
static const uint16_t NUMBER_OF_SAMPLES_COMPASS = 50;
static uint8_t READ_ANGLE_DATA[2];
static dbc_BRIDGE_GPS_s destination_gps_coordinates;
static dbc_BRIDGE_GPS_s current_gps_coordinates;

void compass__read_current_gps_coordinate() {
  gps__run_once();
  gps_coordinates_t current = gps__get_coordinates();
  current_gps_coordinates.BRIDGE_GPS_latitude = current.latitude;
  current_gps_coordinates.BRIDGE_GPS_longitude = current.longitude;
}

static float compass__read_angle_from_i2c_cmps12(void) {
  uint16_t value;
  i2c__read_slave_data(I2C__2, COMPASS_ADDRESS, SLAVE_READ_ANGLE, READ_ANGLE_DATA, 2);
  value = READ_ANGLE_DATA[1];
  value |= READ_ANGLE_DATA[0] << 8;
  float f_value = (float)value / 10.0f;
  return f_value;
}

static bool compass__check_if_cmps12_connected(void) { return i2c__detect(I2C__2, COMPASS_ADDRESS); }

static float compass__read_current_angle(void) {
  uint8_t counter = 1;
  float avg_value = 0;
  if (compass__check_if_cmps12_connected()) {
    while (counter++ % (NUMBER_OF_SAMPLES_COMPASS + 1)) {
      avg_value += compass__read_angle_from_i2c_cmps12();
    }
    avg_value /= NUMBER_OF_SAMPLES_COMPASS;
  }
  return avg_value;
}

static bool check_valid_source_destination_coordinates(dbc_BRIDGE_GPS_s *l_current_gps_coordinates,
                                                       dbc_BRIDGE_GPS_s *l_destination_gps_coordinates) {
  bool flag = true;
  if (l_current_gps_coordinates->BRIDGE_GPS_latitude > 90.0f ||
      l_current_gps_coordinates->BRIDGE_GPS_longitude > 180.0f ||
      l_destination_gps_coordinates->BRIDGE_GPS_latitude > 90.0f ||
      l_destination_gps_coordinates->BRIDGE_GPS_longitude > 180.0f) {
    flag = false;
  }
  return flag;
}

static void convert_source_destination_to_radian(dbc_BRIDGE_GPS_s *l_current_gps_coordinates,
                                                 dbc_BRIDGE_GPS_s *l_destination_gps_coordinates) {
  l_current_gps_coordinates->BRIDGE_GPS_latitude = (l_current_gps_coordinates->BRIDGE_GPS_latitude * PI) / 180;
  l_current_gps_coordinates->BRIDGE_GPS_longitude = (-l_current_gps_coordinates->BRIDGE_GPS_longitude * PI) / 180;
  l_destination_gps_coordinates->BRIDGE_GPS_latitude = (l_destination_gps_coordinates->BRIDGE_GPS_latitude * PI) / 180;
  l_destination_gps_coordinates->BRIDGE_GPS_longitude =
      (-l_destination_gps_coordinates->BRIDGE_GPS_longitude * PI) / 180;
}

float round_upto_2_decimal(float bearing) {
  printf("Data = %f\n", bearing);
  uint64_t value = (uint64_t)(bearing * 100 + 0.5f);
  return (float)value / 100;
}

static float compass__calculate_destination_angle(dbc_BRIDGE_GPS_s l_current_gps_coordinates,
                                                  dbc_BRIDGE_GPS_s l_destination_gps_coordinates) {
  float bearing = 0, lon_difference = 0;
  if (check_valid_source_destination_coordinates(&l_current_gps_coordinates, &l_destination_gps_coordinates)) {
    convert_source_destination_to_radian(&l_current_gps_coordinates, &l_destination_gps_coordinates);
    lon_difference =
        (l_current_gps_coordinates.BRIDGE_GPS_longitude - (l_destination_gps_coordinates.BRIDGE_GPS_longitude));
    bearing = atan2(
        (sin(lon_difference) * cos(l_destination_gps_coordinates.BRIDGE_GPS_latitude)),
        ((cos(l_current_gps_coordinates.BRIDGE_GPS_latitude) * sin(l_destination_gps_coordinates.BRIDGE_GPS_latitude)) -
         (sin(l_current_gps_coordinates.BRIDGE_GPS_latitude) * cos(l_destination_gps_coordinates.BRIDGE_GPS_latitude) *
          cos(lon_difference))));
    bearing = fmodf(((bearing * 180) / PI) + 360, 360);
  }
  bearing = round_upto_2_decimal(bearing);
  return bearing;
}

static float compass__calculate_destination_distance(dbc_BRIDGE_GPS_s current_gps_coordinates,
                                                     dbc_BRIDGE_GPS_s destination_gps_coordinates) {
  float distance = 0.0f;
  if (check_valid_source_destination_coordinates(&current_gps_coordinates, &destination_gps_coordinates)) {
    convert_source_destination_to_radian(&current_gps_coordinates, &destination_gps_coordinates);
    float lat_diff = destination_gps_coordinates.BRIDGE_GPS_latitude - current_gps_coordinates.BRIDGE_GPS_latitude;
    float lon_diff = destination_gps_coordinates.BRIDGE_GPS_longitude - current_gps_coordinates.BRIDGE_GPS_longitude;
    float a = pow(sin(lat_diff / 2), 2) + cos(current_gps_coordinates.BRIDGE_GPS_latitude) *
                                              cos(destination_gps_coordinates.BRIDGE_GPS_latitude) *
                                              pow(sin(lon_diff / 2), 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    distance = 6371 * 1000 * c;
  }
  distance = round_upto_2_decimal(distance);
  return distance;
}
void compass__set_destination_gps(dbc_BRIDGE_GPS_s *copy_dest_data) { destination_gps_coordinates = *copy_dest_data; }

dbc_GEO_COMPASS_s compass__get_current_and_destination_heading(void) {
  dbc_GEO_COMPASS_s compass_data = {};
  compass_data.GEO_COMPASS_current_heading = compass__read_current_angle();
  compass_data.GEO_COMPASS_desitination_heading =
      compass__calculate_destination_angle(current_gps_coordinates, destination_gps_coordinates);
  return compass_data;
}
