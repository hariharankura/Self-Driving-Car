// gps.c
#include "gps.h"

#include "line_buffer.h"
#include "sl_string.h"
#include "uart.h"
#include "uart_printf.h"

#include "FreeRTOS.h"
#include "queue.h"

#include "clock.h" // needed for UART initialization
#include <gpio.h>
#include <stdio.h>
#include <string.h>

static uart_e gps_uart = UART__3;

// Space for the line buffer, and the line buffer data structure instance
static char line_buffer[256];
static line_buffer_s line;

static gps_coordinates_t parsed_coordinates;

static void gps__absorb_data(void) {
  char byte;
  while (uart__get(gps_uart, &byte, 0)) {
    printf("%c", byte);
    line_buffer__add_byte(&line, byte);
  }
}

static bool gps__parse_nema_string(char *gps_nema_string, gps_coordinates_t *temp_coordinates) {
  bool return_value = false;
  float latitude;
  float longitude;
  uint8_t gps_valid;
  char lat_direction = 'N';
  char long_direction = 'E';
  if (*gps_nema_string == '$') {
    if (NULL != strstr(gps_nema_string, "$GPGGA")) {

      sl_string__scanf(gps_nema_string, "$GPGGA, %*f, %f, %c, %f, %c, %hhd, %*s", &latitude, &lat_direction, &longitude,
                       &long_direction, &gps_valid);
      if (long_direction == 'W' || long_direction == 'w') {
        temp_coordinates->longitude = longitude * -1;
      } else {
        temp_coordinates->longitude = longitude;
      }
      printf("Latitude is %c\n", lat_direction);
      if (lat_direction == 'S' || lat_direction == 's') {
        temp_coordinates->latitude = latitude * -1;
      } else {
        temp_coordinates->latitude = latitude;
      }
      return_value = (gps_valid > 0);
    }
  }
  return return_value;
}

static void gps__handle_line(void) {
  char gps_line[100];
  if (line_buffer__remove_line(&line, gps_line, sizeof(gps_line))) {
    gps__parse_nema_string(gps_line, &parsed_coordinates);
  }
}

void gps__uart_pin_function_set() {
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2);
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2);
}

void gps__init(void) {
  gps__uart_pin_function_set();
  line_buffer__init(&line, line_buffer, sizeof(line_buffer));
<<<<<<< HEAD
  uart__init(gps_uart, clock__get_peripheral_clock_hz(), 9600);
=======
  uart__init(gps_uart, clock__get_peripheral_clock_hz(), 38400);
>>>>>>> GEO BOARD: COMPASS ANGLE LOGIC

  QueueHandle_t rxq_handle = xQueueCreate(200, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(8, sizeof(char)); // We don't send anything to the GPS
  uart__enable_queues(gps_uart, rxq_handle, txq_handle);
}

void gps__run_once(void) {
  gps__absorb_data();
  gps__handle_line();
}

gps_coordinates_t gps__get_coordinates(void) { return parsed_coordinates; }