// gps.c
#include "gps.h"

#include "line_buffer.h"
#include "sl_string.h"
#include "uart.h"
#include "uart_printf.h"

#include "FreeRTOS.h"
#include "queue.h"

#include "board_io.h"
#include "clock.h"
#include <gpio.h>
#include <stdio.h>
#include <string.h>

static uart_e gps_uart = UART__3;

static char line_buffer[256];
static line_buffer_s line;

static gps_coordinates_t parsed_coordinates;

static void gps__absorb_data(void) {
  char byte;
  while (uart__get(gps_uart, &byte, 0)) {
    // printf("%c", byte);
    line_buffer__add_byte(&line, byte);
  }
}

static float gps__convert_to_degree(float value) {
  float value_in_degree = (float)((int)(value / 100));
  float value_decimal = (value - value_in_degree * 100) / 60;
  return value_in_degree + value_decimal;
}

static void gps__process_and_save_latitude_longitude(gps_coordinates_t *temp_coordinates, float latitude,
                                                     char lat_direction, float longitude, char long_direction) {
  temp_coordinates->longitude = gps__convert_to_degree(longitude);
  temp_coordinates->latitude = gps__convert_to_degree(latitude);
  if (long_direction == 'W' || long_direction == 'w') {
    temp_coordinates->longitude *= -1;
  }
  if (lat_direction == 'S' || lat_direction == 's') {
    temp_coordinates->latitude *= -1;
  }
}
static bool gps__lock_led(uint8_t gps_valid_check) {
  bool flag = false;
  gpio_s led_op = board_io__get_led3();
  if (gps_valid_check == 1) {
    gpio__set(led_op);
    flag = true;
  } else {
    gpio__reset(led_op);
  }

  return flag;
}
static bool gps__parse_nema_string(char *gps_nema_string, gps_coordinates_t *temp_coordinates) {
  bool return_value = false;
  double latitude = 0;
  double longitude = 0;
  int8_t gps_valid;
  char lat_direction = 'N';
  char long_direction = 'E';
  if (*gps_nema_string == '$') {
    if (NULL != strstr(gps_nema_string, "$GPGGA")) {
      sl_string__scanf(gps_nema_string, "$GPGGA, %*f, %lf, %c, %lf, %c, %hhd, %*s", &latitude, &lat_direction,
                       &longitude, &long_direction, &gps_valid);
      printf("GPS Valid = %d\n", gps_valid);
      printf("GPGA = %s\n", gps_nema_string);
      gps__lock_led(gps_valid);
      gps__process_and_save_latitude_longitude(temp_coordinates, latitude, lat_direction, longitude, long_direction);
      return_value = (gps_valid > 0);
    }
  }
  printf("%f:%f\n", temp_coordinates->latitude, temp_coordinates->longitude);
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
  uart__init(gps_uart, clock__get_peripheral_clock_hz(), 9600);

  QueueHandle_t rxq_handle = xQueueCreate(200, sizeof(char));
  QueueHandle_t txq_handle = xQueueCreate(8, sizeof(char));
  uart__enable_queues(gps_uart, rxq_handle, txq_handle);
}

void gps__run_once(void) {
  gps__absorb_data();
  gps__handle_line();
}

gps_coordinates_t gps__get_coordinates(void) { return parsed_coordinates; }