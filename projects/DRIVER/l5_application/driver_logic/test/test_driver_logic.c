#include "unity.h"

#include "Mockgpio.h"
#include "driver_diagnostics.h"
#include "driver_logic.c"
#include "driving_algo.h"

static const uint16_t obstacle_near_threshold = 29;
static const uint16_t obstacle_far_threshold = 31;

void setUp() {}
void tearDown() {}

void test_driver_logic__process_ultrasonic_sensors_data(void) {
  dbc_SENSOR_USONARS_s test_sensor_data = {.SENSOR_USONARS_left = obstacle_far_threshold,
                                           .SENSOR_USONARS_right = obstacle_far_threshold,
                                           .SENSOR_USONARS_front = obstacle_far_threshold,
                                           .SENSOR_USONARS_back = obstacle_far_threshold};
  test_sensor_data.SENSOR_USONARS_back = 100;
  test_sensor_data.SENSOR_USONARS_front = 200;
  test_sensor_data.SENSOR_USONARS_left = 300;
  test_sensor_data.SENSOR_USONARS_right = 250;
  driver_logic__process_ultrasonic_sensors_data(test_sensor_data);
  TEST_ASSERT_EQUAL_MEMORY(&test_sensor_data, &ultrasonic_sensor_data, sizeof(dbc_SENSOR_USONARS_s));
}
