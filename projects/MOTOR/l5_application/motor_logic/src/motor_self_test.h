#pragma once

#include <stdint.h>

void servo_and_dc_motor_tests(uint32_t callback_count);
uint8_t get_motor_test_button_status(void);
void set_motor_test_button_status(uint8_t status);