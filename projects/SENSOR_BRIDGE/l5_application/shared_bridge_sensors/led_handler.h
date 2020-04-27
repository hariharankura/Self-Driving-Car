#pragma once

#include "ultrasonic_sensor_handler.h"
#include <stdint.h>

void led_handler__turn_off_all_sjtwo_leds(void);

void led_handler__diagnostic_test_object_detection_led3(sensor_t sensor_values);

void led_handler__diagnostic_test_object_detection_leds_for_each_sensor(sensor_t sensor_values);