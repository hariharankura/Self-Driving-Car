#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "unity.h"

// Include the Mocks
// - This will not pull the REAL source code of these modules (such as board_io.c)
// - This will auto-generate "Mock" versions based on the header file
#include "Mockboard_io.h"
#include "Mockcan_handler.h"
#include "Mockgpio.h"
#include "Mockmotor_logic.h"
#include "Mockpwm1.h"
#include "Mockspeed_sensor.h"

// Include the source we wish to test
#include "periodic_callbacks.c"

void setUp(void) {}

void tearDown(void) {}

void test__periodic_callbacks__initialize(void) {
  gpio_s led0, led1, led2, led3;
  init_can_driver_Expect();
  init_pwm_Expect();
  init_speed_sensor_Expect();
  periodic_callbacks__initialize();
}

void test__periodic_callbacks__10Hz(void) {

  can_bus_handler__process_all_received_messages_in_10hz_Expect();
  gpio_s led0;
  board_io__get_led0_ExpectAndReturn(led0);
  gpio__toggle_Expect(led0);
  clear_rotations_in_windowtime_Expect();
  can_bus_handler__transmit_message_in_10hz_Expect();
  periodic_callbacks__10Hz(50);

  can_bus_handler__process_all_received_messages_in_10hz_Expect();
  can_bus_handler__transmit_message_in_10hz_Expect();
  periodic_callbacks__10Hz(51);
}

void test__periodic_callbacks__100Hz(void) {
  pwm1__set_duty_cycle_Expect(PWM1__2_0, 10);
  pwm1__set_duty_cycle_Expect(PWM1__2_1, 10);
  periodic_callbacks__100Hz(0);
}