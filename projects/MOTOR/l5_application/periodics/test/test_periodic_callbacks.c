// #include <stdint.h>
// #include <stdio.h>
// #include <string.h>

// #include "unity.h"

// // Include the Mocks
// // - This will not pull the REAL source code of these modules (such as board_io.c)
// // - This will auto-generate "Mock" versions based on the header file
// #include "Mockacceleration.h"
// #include "Mockboard_io.h"
// #include "Mockcan_handler.h"
// #include "Mockgpio.h"
// #include "Mockmotor_logic.h"
// #include "Mockmotor_self_test.h"
// #include "Mockpwm1.h"
// #include "Mockspeed_sensor.h"

// // Include the source we wish to test
// #include "periodic_callbacks.c"

// void setUp(void) {}

// void tearDown(void) {}

// void test__periodic_callbacks__initialize(void) {
//   gpio_s led0, led1, led2, led3;
//   init_can_driver_Expect();
//   init_pwm_Expect();
//   initialize_speed_sensor_interrupt_Expect();
//   acceleration__init_ExpectAndReturn(true);
//   rc_car_stop_state_Expect();
//   periodic_callbacks__initialize();
// }

// void test__periodic_callbacks__20Hz_Test_Button_not_Pressed(void) {
//   get_motor_test_button_status_ExpectAndReturn(0);
//   can_bus_handler__process_all_received_messages_in_20hz_Expect();
//   can_bus_handler__manage_mia_20hz_Expect();
//   periodic_callbacks__100Hz(15);

//   periodic_callbacks__100Hz(13);
//   periodic_callbacks__100Hz(14);
//   periodic_callbacks__100Hz(16);

//   get_motor_test_button_status_ExpectAndReturn(0);
//   can_bus_handler__process_all_received_messages_in_20hz_Expect();
//   can_bus_handler__manage_mia_20hz_Expect();
//   periodic_callbacks__100Hz(20);
// }

// void test__periodic_callbacks__20Hz_Test_Button_Pressed(void) {
//   get_motor_test_button_status_ExpectAndReturn(1);
//   servo_and_dc_motor_tests_Expect(20);
//   periodic_callbacks__100Hz(20);
// }

// void test__periodic_callbacks__10Hz() {
//   can_bus_handler__transmit_message_in_10hz_Expect();
//   periodic_callbacks__10Hz(1);

//   can_bus_handler__transmit_message_in_10hz_Expect();
//   clear_rotations_in_windowtime_Expect();
//   periodic_callbacks__10Hz(40);
// }
