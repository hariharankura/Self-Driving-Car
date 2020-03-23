#include "periodic_callbacks.h"

#include "board_io.h"
#include "can_bus_handler.h"
#include "compass.h"
#include "gpio.h"
#include "gps.h"
#include <stdio.h>
<<<<<<< HEAD
=======

>>>>>>> GEO BOARD: COMPASS ANGLE LOGIC
/******************************************************************************
 * Your board will reset if the periodic function does not return within its deadline
 * For 1Hz, the function must return within 1000ms
 * For 1000Hz, the function must return within 1ms
 */
<<<<<<< HEAD
void periodic_callbacks__initialize(void) {
  gps__init();
  can_bus_handler__init();
}
=======
void periodic_callbacks__initialize(void) { gps__init(); }
>>>>>>> GEO BOARD: COMPASS ANGLE LOGIC

void periodic_callbacks__1Hz(uint32_t callback_count) {
  // gpio__toggle(board_io__get_led0());
  can_bus_handler__reset_if_bus_off();

  can_bus_handler__process_all_received_messages_in_1hz();
  // Add your code here
}

void periodic_callbacks__10Hz(uint32_t callback_count) {
<<<<<<< HEAD

  compass__read_current_gps_coordinate();
  can_bus_handler__transmit_message_in_10hz();
}
void periodic_callbacks__100Hz(uint32_t callback_count) {
  // gpio__toggle(board_io__get_led2());
=======
  gps__run_once();
  gps_coordinates_t temp_coordinates;
  temp_coordinates = gps__get_coordinates();
  printf("Latitude = %f and Longitude = %f\n", (double)temp_coordinates.latitude, (double)temp_coordinates.longitude);
  can_bus_handler__transmit_message_in_10hz();
  can_bus_handler__process_all_received_messages_in_10hz();
}
void periodic_callbacks__100Hz(uint32_t callback_count) {
  gpio__toggle(board_io__get_led2());
>>>>>>> GEO BOARD: COMPASS ANGLE LOGIC

  // Add your code here
}

/**
 * @warning
 * This is a very fast 1ms task and care must be taken to use this
 * This may be disabled based on intialization of periodic_scheduler__initialize()
 */
void periodic_callbacks__1000Hz(uint32_t callback_count) {
  gpio__toggle(board_io__get_led3());
  // Add your code here
}