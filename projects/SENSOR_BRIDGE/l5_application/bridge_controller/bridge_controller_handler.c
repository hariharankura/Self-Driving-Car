#include "bridge_controller_handler.h"

void bridge_controller_handler__initialize_bluetooth_module(void) {
  // TODO: initialize/setup bluetooth module
}

void bridge_controller_handler__get_destination_coordinates(float *latitude, float *longitude) {
  // hard coded for testing purposes
  *latitude = 112.034;
  *longitude = 4509.1904;

  // TODO: use HC12 bluetooth module to get destination coords from Android phone
}