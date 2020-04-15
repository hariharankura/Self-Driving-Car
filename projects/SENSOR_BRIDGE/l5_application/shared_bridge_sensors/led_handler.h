#pragma once

#include <stdint.h>

void led_handler__turn_off_all_sjtwo_leds(void);

void led_handler__diagnostic_test_object_detection_led3(uint16_t left_sensor, uint16_t right_sensor,
                                                        uint16_t front_sensor, uint16_t back_sensor);

void led_handler__diagnostic_test_object_detection_leds_for_each_sensor(uint16_t left_sensor, uint16_t right_sensor,
                                                                        uint16_t front_sensor, uint16_t back_sensor);